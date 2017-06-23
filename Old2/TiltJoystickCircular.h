#ifndef TILTJOYSTICK_H
#define TILTJOYSTICK_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class TiltJoystick : public Joystick
{
public:
    explicit TiltJoystick()
    : Joystick(),
      _xValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _yValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _tiltTemporaryActivator(Frames(7, 60).getMillis())
    {}

    virtual void update()
    {
        Joystick::update();
        _xValue.update();
        _yValue.update();
        _tiltTemporaryActivator.update();
    }

    virtual void setControls(const Control& xValue, const Control& yValue,
                             const bool tiltCondition)
    {
        Joystick::setControls(xValue, yValue);

        _xValue = xValue;
        _yValue = yValue;

        bool tiltResetCondition = justActivated() || justDeactivated();

        _tiltTemporaryActivator.setControls(tiltResetCondition);

        bool atLeastOneAxisIsActive = getXControl().isActive() || getYControl().isActive();
        bool tiltShouldHappen = tiltCondition && atLeastOneAxisIsActive && _tiltTemporaryActivator;

        if (tiltShouldHappen)
            clampDistance(0.2);
    }

private:
    Control _xValue;
    Control _yValue;

    TemporaryActivator _tiltTemporaryActivator;

    void clampDistance(const double newDistance)
    {
        double oldX = getXControl();
        double oldY = getYControl();

        double oldDistance = calculateDistance(oldX, oldY);

        double newX = newDistance * oldX / oldDistance;
        double newY = newDistance * oldY / oldDistance;

        _xValue = newX;
        _yValue = newY;

        Joystick::setControls(_xValue, _yValue);
    }
};


#endif // TILTJOYSTICK_H
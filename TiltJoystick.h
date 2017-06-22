#ifndef TILTJOYSTICK_H
#define TILTJOYSTICK_H

#include "Joystick.h"
#include "TemporaryActivator.h"

class TiltJoystick : public Joystick
{
public:
    explicit TiltJoystick(const Control& xControl, const Control& yControl,
                          const Control& tilt)
    : Joystick(_xValue, _yValue),
      _xValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _yValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _xControl(xControl),
      _yControl(yControl),
      _tilt(tilt)
    {}

    virtual void update()
    {
        _xValue.update();
        _yValue.update();

        _xValue.setValue(_xControl.getControl());
        _yValue.setValue(_yControl.getControl());

        bool atLeastOneAxisIsActive = _xControl.isActive() || _yControl.isActive();
        bool tiltShouldHappen = _tilt.isActive() && atLeastOneAxisIsActive;

        if (tiltShouldHappen)
            clampDistance(0.2);

        Joystick::update();
    }

private:
    ControlSlot _tilt;
    ControlSlot _xControl;
    ControlSlot _yControl;

    Control _xValue;
    Control _yValue;

    void clampDistance(const double newDistance)
    {
        double oldX = _xValue.getValue();
        double oldY = _yValue.getValue();

        double oldDistance = calculateDistance(oldX, oldY);

        double newX = newDistance * oldX / oldDistance;
        double newY = newDistance * oldY / oldDistance;

        _xValue.setValue(newX);
        _yValue.setValue(newY);
    }
};


#endif // TILTJOYSTICK_H
#ifndef TILTJOYSTICK_H
#define TILTJOYSTICK_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class TiltJoystick : public Joystick
{
public:
    TiltJoystick()
    : Joystick(),
      _xValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _yValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _tiltTemporaryActivator(Frames(5, 60).getMillis()),
      _tiltAmount(0.4)
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
        _xValue = xValue;
        _yValue = yValue;

        bool tiltResetX = _xValue.justActivated() || _xValue.justDeactivated() || _xValue.hasCrossedInactiveRange();
        bool tiltResetY = _yValue.justActivated() || _yValue.justDeactivated() || _yValue.hasCrossedInactiveRange();
        bool tiltResetCondition = tiltResetX || tiltResetY;

        _tiltTemporaryActivator.setControls(tiltResetCondition);

        bool atLeastOneAxisIsActive = _xValue.isActive() || _yValue.isActive();
        bool tiltShouldHappen = tiltCondition && atLeastOneAxisIsActive && _tiltTemporaryActivator;

        if (tiltShouldHappen)
        {
            clampAxis(_xValue);
            clampAxis(_yValue);
        }

        Joystick::setControls(_xValue, _yValue);
    }

private:
    Control _xValue;
    Control _yValue;

    TemporaryActivator _tiltTemporaryActivator;

    const double _tiltAmount;

    void clampAxis(Control& axis)
    {
        double axisValue = axis.getValue();

        double lowClamp = -_tiltAmount;
        double highClamp = _tiltAmount;

        if (axisValue < lowClamp)
        {
            axis = lowClamp;
            return;
        }

        if (axisValue > highClamp)
        {
            axis = highClamp;
            return;
        }
    }
};


#endif // TILTJOYSTICK_H
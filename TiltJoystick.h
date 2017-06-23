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
      _tiltTemporaryActivator(Frames(6, 60).getMillis()),
      _tiltAmount(0.3)
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

        bool tiltResetX = xValue.justActivated() || xValue.justDeactivated() || xValue.hasCrossedInactiveRange();
        bool tiltResetY = yValue.justActivated() || yValue.justDeactivated() || yValue.hasCrossedInactiveRange();
        bool tiltResetCondition = tiltResetX || tiltResetY;

        _tiltTemporaryActivator.setControls(tiltResetCondition);

        bool atLeastOneAxisIsActive = getXControl().isActive() || getYControl().isActive();
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
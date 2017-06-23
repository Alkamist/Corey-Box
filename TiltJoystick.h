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
      _tiltX(frames(7)),
      _tiltY(frames(7)),
      _tiltAmount(0.4)
    {}

    virtual void update()
    {
        Joystick::update();
        _xValue.update();
        _yValue.update();
        _tiltX.update();
        _tiltY.update();
    }

    virtual void setControls(const Control& xValue, const Control& yValue,
                             const bool tiltCondition)
    {
        _xValue = xValue;
        _yValue = yValue;

        bool tiltResetX = _xValue.justActivated() || _xValue.justDeactivated() || _xValue.hasCrossedInactiveRange();
        bool tiltResetY = _yValue.justActivated() || _yValue.justDeactivated() || _yValue.hasCrossedInactiveRange();

        _tiltX.setControls(tiltResetX);
        _tiltY.setControls(tiltResetY);

        if (_tiltX && tiltCondition)
            clampAxis(_xValue);

        if (_tiltY && tiltCondition)
            clampAxis(_yValue);

        Joystick::setControls(_xValue, _yValue);
    }

private:
    Control _xValue;
    Control _yValue;

    TemporaryActivator _tiltX;
    TemporaryActivator _tiltY;

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
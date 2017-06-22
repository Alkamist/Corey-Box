#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "TiltJoystick.h"
#include "DoubleModAxis.h"
#include "Signum.h"

class ButtonOnlyLeftStick : public TiltJoystick
{
public:
    explicit ButtonOnlyLeftStick(const Control& left, const Control& right,
                                 const Control& down, const Control& up,
                                 const Control& xMod1, const Control& xMod2,
                                 const Control& yMod1, const Control& yMod2,
                                 const Control& tilt, const Control& tiltTempDisable,
                                 const Control& shieldDrop)
    : TiltJoystick(_xAxis, _yAxis, tilt),
      _left(left),
      _right(right),
      _down(down),
      _up(up),
      _xMod1(xMod1),
      _xMod2(xMod2),
      _yMod1(yMod1),
      _yMod2(yMod2),
      _xAxis(left, right,
             xMod1, xMod2),
      _yAxis(down, up,
             yMod1, yMod2)
    {}

    virtual void update()
    {
        _xAxis.update();
        _yAxis.update();

        bool upLeft = _left.isActive() && _up.isActive();
        bool upRight = _right.isActive() && _up.isActive();
        bool downLeft = _left.isActive() && _down.isActive();
        bool downRight = _right.isActive() && _down.isActive();

        bool anyAngle = upLeft || upRight || downLeft || downRight;

        bool anyXMod = _xMod1.isActive() || _xMod2.isActive();
        bool anyYMod = _yMod1.isActive() || _yMod2.isActive();

        if (anyAngle && anyXMod && !anyYMod)
            calculateOtherAxis(_xAxis, _yAxis);

        if (anyAngle && anyYMod && !anyXMod)
            calculateOtherAxis(_yAxis, _xAxis);

        TiltJoystick::update();
    }

private:
    ControlSlot _left;
    ControlSlot _right;
    ControlSlot _down;
    ControlSlot _up;

    ControlSlot _xMod1;
    ControlSlot _xMod2;
    ControlSlot _yMod1;
    ControlSlot _yMod2;

    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;

    //Control _tiltActivator;

    void calculateOtherAxis(Control& axisToSet, Control& otherAxis)
    {
        double valueSign = signum(otherAxis.getValue());
        double axisMagnitude = sqrt(1.0 - square(axisToSet.getValue()));
        double newValue = valueSign * axisMagnitude;
        otherAxis.setValue(newValue);
    }
};


#endif // BUTTONONLYLEFTSTICK_H
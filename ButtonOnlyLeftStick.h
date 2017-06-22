#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "Joystick.h"
#include "TwoButtonControl.h"

class ButtonOnlyLeftStick : public Joystick
{
public:
    ButtonOnlyLeftStick(const Control& left,
                        const Control& right,
                        const Control& down,
                        const Control& up,
                        const Control& xMod1,
                        const Control& xMod2,
                        const Control& yMod1,
                        const Control& yMod2,
                        const Control& shieldDrop,
                        const Control& tilt)
    : Joystick(_xAxis, _yAxis),
      _xAxis(left, right),
      _yAxis(down, up),
      _xMod1(xMod1),
      _xMod2(xMod2),
      _yMod1(yMod1),
      _yMod2(yMod2),
      _shieldDrop(shieldDrop),
      _tilt(tilt)
    {}

    virtual void update()
    {
        Joystick::update();

        _xAxis.update();
        _yAxis.update();
    }

private:
    TwoButtonControl _xAxis;
    TwoButtonControl _yAxis;

    ControlSlot _xMod1;
    ControlSlot _xMod2;
    ControlSlot _yMod1;
    ControlSlot _yMod2;
    ControlSlot _shieldDrop;
    ControlSlot _tilt;
};


#endif // BUTTONONLYLEFTSTICK_H
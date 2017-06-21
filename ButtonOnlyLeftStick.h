#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "Joystick.h"
#include "TwoButtonControl.h"

class ButtonOnlyLeftStick : public Joystick
{
public:
    ButtonOnlyLeftStick()
    : Joystick()
    {}

    void setControls(const Control& left,
                     const Control& right,
                     const Control& down,
                     const Control& up,
                     const Control& xMod1,
                     const Control& xMod2,
                     const Control& yMod1,
                     const Control& yMod2,
                     const Control& shieldDrop,
                     const Control& tilt)
    {}

private:
    TwoButtonControl _xAxis;
    TwoButtonControl _yAxis;

    Control _xMod1;
    Control _xMod2;
    Control _yMod1;
    Control _yMod2;
    Control _shieldDrop;
    Control _tilt;
};


#endif // BUTTONONLYLEFTSTICK_H
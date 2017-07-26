#ifndef ANALOGLEFTSTICK_H
#define ANALOGLEFTSTICK_H

#include "FightingJoystick.h"
#include "JoystickShieldDropper.h"
#include "Signum.h"

// This class represents the left analog stick used in the button only controller.
class AnalogLeftStick : public FightingJoystick
{
public:
    AnalogLeftStick()
    : FightingJoystick()
    {}

    void process()
    {
        Joystick::process();

        _joystickShieldDropper.process(*this);
    }

    void endCycle()
    {
        Joystick::endCycle();
    }

    void setShieldState(const bool state)             { _joystickShieldDropper.setShieldState(state); }

    void setXValue(const uint8_t value)               { xValue = value; }
    void setYValue(const uint8_t value)               { yValue = value; }

    void setShieldDrop(const uint8_t value)           { _joystickShieldDropper.setShieldDrop(value); }
    void resetShieldDrop()                            { _joystickShieldDropper.resetShieldDrop(); }

private:
    JoystickTilter _joystickTilter;
    JoystickShieldDropper _joystickShieldDropper;
};

#endif // ANALOGLEFTSTICK_H

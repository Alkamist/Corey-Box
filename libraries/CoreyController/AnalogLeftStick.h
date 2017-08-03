#ifndef ANALOGLEFTSTICK_H
#define ANALOGLEFTSTICK_H

#include "FightingJoystick.h"
#include "JoystickShieldDropper.h"

class AnalogLeftStick : public FightingJoystick
{
public:
    AnalogLeftStick()
    : FightingJoystick(),
      _shieldState(false)
    {}

    void process()
    {
        Joystick::process();

        _joystickShieldDropper.setShieldDropState((yValue < _joystickShieldDropper.getShieldDrop())
                                                  && _shieldState);

        _joystickShieldDropper.process(*this);
    }

    void endCycle()
    {
        Joystick::endCycle();
    }

    void setShieldState(const bool state)   { _joystickShieldDropper.setShieldState(state); _shieldState = state; }

    void setXValue(const uint8_t value)     { xValue = value; }
    void setYValue(const uint8_t value)     { yValue = value; }

    void setShieldDrop(const uint8_t value) { _joystickShieldDropper.setShieldDrop(value); }

private:
    bool _shieldState;

    JoystickShieldDropper _joystickShieldDropper;
};

#endif // ANALOGLEFTSTICK_H

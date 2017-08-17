#ifndef JOYSTICKSHIELDDROPPER_H
#define JOYSTICKSHIELDDROPPER_H

#include "Joystick.h"

// This class will allow for a shield drop when the two input conditions
// are met.
class JoystickShieldDropper
{
public:
    JoystickShieldDropper()
    : _shieldDropState(false),
      _shieldState(false),
      _shieldDropValue(74)
    {}

    void process(Joystick& joystick)
    {
        if (_shieldDropState && _shieldState)
        {
            joystick.xValue = 128;
            joystick.yValue = _shieldDropValue;
        }

        joystick.Joystick::process();
    }

    void setShieldDropState(const bool state) { _shieldDropState = state; }
    void setShieldState(const bool state)     { _shieldState = state; }

    void setShieldDrop(const uint8_t value)   { _shieldDropValue = value; }
    const uint8_t getShieldDrop() const       { return _shieldDropValue; }

private:
    bool _shieldDropState;
    bool _shieldState;

    uint8_t _shieldDropValue;
};

#endif // JOYSTICKSHIELDDROPPER_H
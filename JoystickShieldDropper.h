#ifndef JOYSTICKSHIELDDROPPER_H
#define JOYSTICKSHIELDDROPPER_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

// This class will allow for a shield drop when the two input conditions
// are met.
class JoystickShieldDropper
{
public:
    JoystickShieldDropper()
    : _shieldDropState(false),
      _shieldState(false),
      _shieldDropValue(0)
    {
        resetShieldDrop();
    }

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

    void resetShieldDrop()
    {
        _shieldDropValue = 42;
    }

    void setShieldDrop(const uint8_t value)
    {
        _shieldDropValue = value;
    }

private:
    bool _shieldDropState;
    bool _shieldState;

    uint8_t _shieldDropValue;
};

#endif // JOYSTICKSHIELDDROPPER_H

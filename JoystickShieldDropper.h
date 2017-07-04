#ifndef JOYSTICKSHIELDDROPPER_H
#define JOYSTICKSHIELDDROPPER_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class JoystickShieldDropper
{
public:
    JoystickShieldDropper()
    : _shieldDropState(false),
      _shieldState(false),
      _shieldDropValue(-0.67500)
    {}

    void process(Joystick& joystick)
    {
        if (_shieldDropState && _shieldState)
        {
            joystick.setXValue(0.0);
            joystick.setYValue(_shieldDropValue);
        }

        joystick.Joystick::process();
    }

    void setShieldDropState(const bool state) { _shieldDropState = state; }
    void setShieldState(const bool state)     { _shieldState = state; }

    void trimShieldDropDown()
    {
         _shieldDropValue -= 0.015;

        if (_shieldDropValue < -1.0)
            _shieldDropValue = -1.0;
    }

    void trimShieldDropUp()
    {
        _shieldDropValue += 0.015;

        if (_shieldDropValue > 0.0)
            _shieldDropValue = 0.0;
    }

    void resetShieldDrop()
    {
        _shieldDropValue = -0.67500;
    }

    void setShieldDrop(const float value)
    {
        _shieldDropValue = value;
    }

private:
    bool _shieldDropState;
    bool _shieldState;

    float _shieldDropValue;
};

#endif // JOYSTICKSHIELDDROPPER_H

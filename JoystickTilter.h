#ifndef JOYSTICKTILTER_H
#define JOYSTICKTILTER_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class JoystickTilter
{
public:
    JoystickTilter()
    : _tiltState(false),
      _tiltAmount(77)
    {
        _tiltX.setTime(frames(7));
        _tiltY.setTime(frames(7));
    }

    void process(FightingJoystick& joystick)
    {
        bool tiltResetX = joystick.xIsInDeadZone()
                       || joystick.xValue.justCrossedCenter();

        bool tiltResetY = joystick.yIsInDeadZone()
                       || joystick.yValue.justCrossedCenter();

        _tiltX = tiltResetX;
        _tiltY = tiltResetY;

        _tiltX.process();
        _tiltY.process();

        if (_tiltX && _tiltState)
            joystick.xValue = clampValue(joystick.xValue);

        if (_tiltY && _tiltState)
            joystick.yValue = clampValue(joystick.yValue);

        joystick.FightingJoystick::process();
    }

    void endCycle()
    {
        _tiltX.endCycle();
        _tiltY.endCycle();
    }

    void setTiltState(const bool state)  { _tiltState = state; }

private:
    bool _tiltState;

    TemporaryActivator _tiltX;
    TemporaryActivator _tiltY;

    const uint8_t _tiltAmount;

    const uint8_t clampValue(const uint8_t value) const
    {
        uint8_t lowClamp = 128 - _tiltAmount;
        uint8_t highClamp = 128 + _tiltAmount;

        if (value < lowClamp)
            return lowClamp;

        if (value > highClamp)
            return highClamp;

        return value;
    }
};

#endif // JOYSTICKTILTER_H

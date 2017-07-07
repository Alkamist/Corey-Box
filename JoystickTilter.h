#ifndef JOYSTICKTILTER_H
#define JOYSTICKTILTER_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"
#include "ClampBipolar.h"

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
            joystick.xValue = clampBipolar(joystick.xValue, _tiltAmount);

        if (_tiltY && _tiltState)
            joystick.yValue = clampBipolar(joystick.yValue, _tiltAmount);

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
};

#endif // JOYSTICKTILTER_H

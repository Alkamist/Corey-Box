#ifndef JOYSTICKTILTER_H
#define JOYSTICKTILTER_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"
#include "ClampBipolar.h"

// This class provides the capability to force a joystick to move slowly.
// This will prevent dash, rolling, spotdodging, and shield-dropping when activated.
class JoystickTilter
{
public:
    JoystickTilter()
    : _tiltState(false),
      _tiltAmount(0)
    {
        resetTilt();

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

    void setTilt(const uint8_t value)    { _tiltAmount = value; }
    void resetTilt()                     { _tiltAmount = 77; }

private:
    bool _tiltState;

    TemporaryActivator _tiltX;
    TemporaryActivator _tiltY;

    uint8_t _tiltAmount;
};

#endif // JOYSTICKTILTER_H
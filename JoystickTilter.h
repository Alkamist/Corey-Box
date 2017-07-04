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
      _tiltAmount(0.60)
    {
        _tiltXOut.setTime(frames(7.0));
        _tiltYOut.setTime(frames(7.0));
    }

    void process(Joystick& joystick)
    {
        bool tiltResetX = joystick.xIsInDeadZone()
                       || joystick.getXControl().justCrossedInactiveRange();

        bool tiltResetY = joystick.yIsInDeadZone()
                       || joystick.getYControl().justCrossedInactiveRange();

        _tiltXOut.setActivatorState(tiltResetX);
        _tiltYOut.setActivatorState(tiltResetY);

        _tiltXOut.process();
        _tiltYOut.process();

        if (_tiltXOut && _tiltState)
            joystick.setXValue(clampValue(joystick.getXValue()));

        if (_tiltYOut && _tiltState)
            joystick.setYValue(clampValue(joystick.getYValue()));

        joystick.Joystick::process();
    }

    void endCycle()
    {
        _tiltXOut.endCycle();
        _tiltYOut.endCycle();
    }

    void setTiltState(const bool state)  { _tiltState = state; }

private:
    bool _tiltState;

    TemporaryActivator _tiltXOut;
    TemporaryActivator _tiltYOut;

    const float _tiltAmount;

    const float clampValue(const float value) const
    {
        float lowClamp = -_tiltAmount;
        float highClamp = _tiltAmount;

        if (value < lowClamp)
            return lowClamp;

        if (value > highClamp)
            return highClamp;

        return value;
    }
};

#endif // JOYSTICKTILTER_H

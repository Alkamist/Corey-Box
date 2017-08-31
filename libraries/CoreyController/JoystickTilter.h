#ifndef JOYSTICKTILTER_H
#define JOYSTICKTILTER_H

#include "Joystick.h"
#include "ScaleBipolar.h"
#include "Signum.h"
#include "TemporaryActivator.h"

// This class provides the capability to force a joystick to move slowly.
// This will prevent dash, rolling, spotdodging, and shield-dropping when activated.
class JoystickTilter
{
public:
    JoystickTilter()
    : _tiltState(false),
      _useMacros(false),
      _tiltAmount(49),
      _range(127)
    {
        _tiltActivator.setTime(16);
    }

    void process(FightingJoystick& joystick)
    {
        bool tiltResetX = joystick.xJustLeftDeadZone()
                       || joystick.xValue.justCrossedCenter();

        bool tiltResetY = joystick.yJustLeftDeadZone()
                       || joystick.yValue.justCrossedCenter();

        _tiltActivator = tiltResetX || tiltResetY;
        _tiltActivator.process();

        if ((_tiltActivator || !_useMacros) && _tiltState)
        {
            joystick.xValue = scaleBipolar(joystick.xValue, _tiltAmount, _range);

            if (joystick.xIsInDeadZone())
            {
                if (joystick.xValue < 128) joystick.xValue = 128 - joystick.getDeadZoneUpperBound() - 1;
                if (joystick.xValue > 128) joystick.xValue = 128 + joystick.getDeadZoneUpperBound() + 1;
            }

            joystick.yValue = scaleBipolar(joystick.yValue, _tiltAmount, _range);

            if (joystick.yIsInDeadZone())
            {
                if (joystick.yValue < 128) joystick.yValue = 128 - joystick.getDeadZoneUpperBound() - 1;
                if (joystick.yValue > 128) joystick.yValue = 128 + joystick.getDeadZoneUpperBound() + 1;
            }
        }

        joystick.FightingJoystick::process();
    }

    void endCycle()
    {
        _tiltActivator.endCycle();
    }

    void setUseMacros(const bool state)  { _useMacros = state; }
    void setTiltState(const bool state)  { _tiltState = state; }
    void setTilt(const uint8_t value)    { _tiltAmount = value; }
    void setRange(const uint8_t value)   { _range = value; }

private:
    bool _tiltState;
    bool _useMacros;

    uint8_t _tiltAmount;
    uint8_t _range;

    TemporaryActivator _tiltActivator;
};

#endif // JOYSTICKTILTER_H
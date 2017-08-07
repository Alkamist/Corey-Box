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
      _tiltAmount(49),
      _range(127)
    {
        _tiltX.setTime(8);
        _tiltY.setTime(8);
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
        {
            joystick.xValue = scaleBipolar(joystick.xValue, _tiltAmount, _range);

            if (joystick.xIsInDeadZone())
            {
                if (joystick.xValue < 128) joystick.xValue = 128 - joystick.getDeadZoneUpperBound() - 1;
                if (joystick.xValue > 128) joystick.xValue = 128 + joystick.getDeadZoneUpperBound() + 1;
            }
        }

        if (_tiltY && _tiltState)
        {
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
        _tiltX.endCycle();
        _tiltY.endCycle();
    }

    void setTiltState(const bool state)  { _tiltState = state; }

    void setTilt(const uint8_t value)    { _tiltAmount = value; }

    void setRange(const uint8_t value)   { _range = value; }

private:
    bool _tiltState;

    uint8_t _tiltAmount;
    uint8_t _range;

    TemporaryActivator _tiltX;
    TemporaryActivator _tiltY;
};

#endif // JOYSTICKTILTER_H

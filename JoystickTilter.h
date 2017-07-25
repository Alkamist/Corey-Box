#ifndef JOYSTICKTILTER_H
#define JOYSTICKTILTER_H

#include "Joystick.h"
#include "ScaleBipolar.h"
#include "Signum.h"

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
    }

    void process(FightingJoystick& joystick)
    {
        if (_tiltState)
        {
            joystick.xValue = scaleBipolar(joystick.xValue, _tiltAmount);
            joystick.yValue = scaleBipolar(joystick.yValue, _tiltAmount);

            if (joystick.xIsInDeadZone())
            {
                if (joystick.xValue < 128) joystick.xValue = 128 - joystick.getDeadZoneUpperBound() - 1;
                if (joystick.xValue > 128) joystick.xValue = 128 + joystick.getDeadZoneUpperBound() + 1;
            }

            if (joystick.yIsInDeadZone())
            {
                if (joystick.yValue < 128) joystick.yValue = 128 - joystick.getDeadZoneUpperBound() - 1;
                if (joystick.yValue > 128) joystick.yValue = 128 + joystick.getDeadZoneUpperBound() + 1;
            }
        }

        joystick.FightingJoystick::process();
    }

    void setTiltState(const bool state)  { _tiltState = state; }

    void setTilt(const uint8_t value)    { _tiltAmount = value; }
    void resetTilt()                     { _tiltAmount = 84; }

private:
    bool _tiltState;

    uint8_t _tiltAmount;
};

#endif // JOYSTICKTILTER_H

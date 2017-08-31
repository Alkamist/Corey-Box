#ifndef JOYSTICKTILTER_H
#define JOYSTICKTILTER_H

#include "Joystick.h"
#include "ScaleBipolar.h"
#include "Signum.h"
#include "TemporaryActivator.h"

// This class provides the capability to limit a joystick to a certain range.
// This will prevent dash, rolling, spotdodging, and shield-dropping when activated.
class JoystickTilter
{
public:
    JoystickTilter()
    : _tiltState(false),
      _useMacros(false),
      _tiltAmount(49),
      _range(127)
    {}

    void process(FightingJoystick& joystick)
    {
        if (_tiltState)
        {
            joystick.xValue = scaleBipolar(joystick.xValue, _tiltAmount, _range);
            joystick.yValue = scaleBipolar(joystick.yValue, _tiltAmount, _range);

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

    void endCycle() {}

    void setUseMacros(const bool state)  { _useMacros = state; }
    void setTiltState(const bool state)  { _tiltState = state; }
    void setTilt(const uint8_t value)    { _tiltAmount = value; }
    void setRange(const uint8_t value)   { _range = value; }

private:
    bool _tiltState;
    bool _useMacros;

    uint8_t _tiltAmount;
    uint8_t _range;
};

#endif // JOYSTICKTILTER_H
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
    : _tiltAmount(49),
      _range(127)
    {
        _tiltXActivator.setTime(16);
        _tiltYActivator.setTime(16);
    }

    void process(FightingJoystick& joystick)
    {
        bool tiltResetX = joystick.xJustLeftDeadZone()
                       || joystick.xValue.justCrossedCenter();

        bool tiltResetY = joystick.yJustLeftDeadZone()
                       || joystick.yValue.justCrossedCenter();

        _tiltXActivator = tiltResetX;
        _tiltXActivator.process();

        _tiltYActivator = tiltResetY;
        _tiltYActivator.process();

        // Tilt X
        if ((_tiltXActivator || !_useMacros) && _tiltState)
        {
            joystick.xValue = scaleBipolar(joystick.xValue, _tiltAmount, _range);

            // Don't let the tilt scaling force values into the deadzone unintentionally.
            if (joystick.xIsInDeadZone())
            {
                if (joystick.xValue < 128) joystick.xValue = 128 - joystick.getDeadZoneUpperBound() - 1;
                if (joystick.xValue > 128) joystick.xValue = 128 + joystick.getDeadZoneUpperBound() + 1;
            }
        }

        // Tilt Y
        if ((_tiltYActivator || !_useMacros) && _tiltState && !_tiltYDisable)
        {
            joystick.yValue = scaleBipolar(joystick.yValue, _tiltAmount, _range);

            // Don't let the tilt scaling force values into the deadzone unintentionally.
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
        _tiltState.endCycle();
        _shieldState.endCycle();
        _wavedashState.endCycle();
        _lsDownState.endCycle();
        _useMacros.endCycle();

        _tiltXActivator.endCycle();
        _tiltYActivator.endCycle();
    }

    void setUseMacros(const bool state)     { _useMacros = state; }

    void setTiltState(const bool state)     { _tiltState = state; }
    void setShieldState(const bool state)   { _shieldState = state; }
    void setWavedashState(const bool state) { _wavedashState = state; }
    void setLsDownState(const bool state)   { _lsDownState = state; }

    void setTilt(const uint8_t value)       { _tiltAmount = value; }
    void setRange(const uint8_t value)      { _range = value; }

    void setTiltYDisable(const bool state)  { _tiltYDisable = state; }

    const bool xTiltIsFinished() const      { return !_tiltXActivator; }

private:
    Activator _tiltState;
    Activator _shieldState;
    Activator _wavedashState;
    Activator _lsDownState;
    Activator _useMacros;

    uint8_t _tiltAmount;
    uint8_t _range;

    TemporaryActivator _tiltXActivator;
    TemporaryActivator _tiltYActivator;
};

#endif // JOYSTICKTILTER_H
#ifndef EIGHTWAYLEFTSTICK_H
#define EIGHTWAYLEFTSTICK_H

#include "FightingJoystick.h"
#include "SingleModAxis.h"
#include "Signum.h"
#include "TemporaryActivator.h"
#include "ScaleBipolar.h"
#include "TwoButtonSpamMacro.h"
#include "TwoButtonStateTracker.h"

// This class represents the left analog stick used in the eight way controller.
class EightWayLeftStick : public FightingJoystick
{
public:
    EightWayLeftStick()
    : FightingJoystick(),
      _tiltAmount(49),
      _range(127),
      _shieldDropValue(74),
      _ABTiltValue(100)
    {
        _tiltXActivator.setTime(16);
        _tiltYActivator.setTime(16);
        _forceWaveland.setTime(8);
    }

    void process()
    {
        _xAxis.process();
        _yAxis.process();

        xValue = _xAxis;
        yValue = _yAxis;

        Joystick::process();

        handleWavelands();
        handleWavedashes();
        handleShieldDropping();
        handleTilting();
        handleABAngles();
        handleSmashDI();
    }

    // Force perfect wavelands when holding straight left or right.
    void handleWavelands()
    {
        bool straightLeftOrRight = (_lsLeftState || _lsRightState) && !(_lsDownState || _lsUpState);

        _forceWaveland = _shieldState.justActivated();
        _forceWaveland.process();

        if (_shieldState && straightLeftOrRight && _forceWaveland)
            yValue = 128 - _yAxis.getModValue();

        Joystick::process();
    }

    // Force perfect wavedashes when holding straight left or right.
    // Also force wavedash down when the stick is neutral.
    void handleWavedashes()
    {
        if (_wavedashState)
        {
            if (yValue > (128 - getTiltLowerBound()))
                yValue = 128 - _yAxis.getModValue();
        }

        Joystick::process();
    }

    void handleShieldDropping()
    {
        bool shieldDropCondition = _shieldDropState && _shieldState;
        bool shieldDropDisable = _wavedashState || _tiltState || _xModState || _yModState;

        if (shieldDropCondition && !shieldDropDisable)
        {
            if (yValue < _shieldDropValue)
                yValue = _shieldDropValue;
        }

        Joystick::process();
    }

    void handleTilting()
    {
        bool passiveYTilt = _lsUpState && !_BState;
        bool tiltCondition = _tiltState || _shieldState || _xModState || _yModState || passiveYTilt;
        bool disableTilt = _wavedashState || (_BState && _shieldState);

        bool tiltResetX = (xJustLeftDeadZone() || xValue.justCrossedCenter()) && tiltCondition;
        bool tiltResetY = (yJustLeftDeadZone() || yValue.justCrossedCenter()) && tiltCondition;

        _tiltXActivator = tiltResetX || _shieldState.justActivated();
        _tiltXActivator.process();

        _tiltYActivator = tiltResetY || _shieldState.justActivated();
        _tiltYActivator.process();

        bool tiltActivator = _tiltXActivator || _tiltYActivator;

        // Tilt X
        if (tiltActivator && tiltCondition && !disableTilt)
        {
            xValue = scaleBipolar(xValue, _tiltAmount, _range);

            // Don't let the tilt scaling force values into the deadzone unintentionally.
            if (xIsInDeadZone())
            {
                if (xValue < 128) xValue = 128 - getDeadZoneUpperBound() - 1;
                if (xValue > 128) xValue = 128 + getDeadZoneUpperBound() + 1;
            }
        }

        bool disableYTilt = _shieldState && _lsDownState && !_tiltState && !_xModState && !_yModState && !_lsUpState;

        // Tilt Y
        if (tiltActivator && tiltCondition && !disableTilt && !disableYTilt)
        {
            yValue = scaleBipolar(yValue, _tiltAmount, _range);

            // Don't let the tilt scaling force values into the deadzone unintentionally.
            if (yIsInDeadZone())
            {
                if (yValue < 128) yValue = 128 - getDeadZoneUpperBound() - 1;
                if (yValue > 128) yValue = 128 + getDeadZoneUpperBound() + 1;
            }
        }

        FightingJoystick::process();
    }

    void handleABAngles()
    {
        bool AOrB = _AState || _BState;

        bool onlyLeft = _lsLeftState && !(_lsUpState || _lsDownState);
        bool onlyRight = _lsRightState && !(_lsUpState || _lsDownState);
        //bool ABTiltDisabler = _jumpState || _shieldState || _wavedashState
        //                   || _xModState || _yModState || onlyLeft || onlyRight;
        bool ABTiltDisabler = _xModState || _yModState || onlyLeft || onlyRight;

        if (AOrB && !ABTiltDisabler)
        {
            xValue = scaleBipolar(xValue, _ABTiltValue);
        }

        FightingJoystick::process();
    }

    void handleSmashDI()
    {
        bool leftOrRight = _lsLeftState || _lsRightState;
        bool downOrUp = _lsDownState || _lsUpState;
        bool activateSmashDI = (leftOrRight || downOrUp) && _smashDIState;

        _smashDIMacro = activateSmashDI;
        _smashDIMacro.process();

        if (_smashDIMacro.isRunning())
        {
            if (_smashDIMacro.getButton1())
            {
                if (_lsLeftState)
                    xValue = 128 - _range;
                if (_lsRightState)
                    xValue = 128 + _range;
            }
            else
            {
                xValue = 128;
            }

            if (_smashDIMacro.getButton2())
            {
                if (_lsDownState)
                    yValue = 128 - _range;
                if (_lsUpState)
                    yValue = 128 + _range;
            }
            else
            {
                yValue = 128;
            }
        }

        FightingJoystick::process();
    }

    void endCycle()
    {
        Joystick::endCycle();

        _tiltState.endCycle();
        _shieldState.endCycle();
        _shieldDropState.endCycle();
        _wavedashState.endCycle();
        _lsDownState.endCycle();
        _lsUpState.endCycle();
        _lsLeftState.endCycle();
        _lsRightState.endCycle();
        _jumpState.endCycle();
        _xModState.endCycle();
        _yModState.endCycle();
        _AState.endCycle();
        _BState.endCycle();
        _smashDIState.endCycle();

        _tiltXActivator.endCycle();
        _tiltYActivator.endCycle();
        _forceWaveland.endCycle();

        _xAxis.endCycle();
        _yAxis.endCycle();

        _smashDIMacro.endCycle();
    }

    void setLsLeftState(const bool state)             { _lsLeftState = state; _xAxis.setLowState(state); }
    void setLsRightState(const bool state)            { _lsRightState = state; _xAxis.setHighState(state); }
    void setLsDownState(const bool state)             { _lsDownState = state; _yAxis.setLowState(state); }
    void setLsUpState(const bool state)               { _lsUpState = state; _yAxis.setHighState(state); }
    void setXModState(const bool state)               { _xModState = state; _xAxis.setModState(state); }
    void setYModState(const bool state)               { _yModState = state; _yAxis.setModState(state); }

    void setTiltState(const bool state)               { _tiltState = state; }
    void setWavedashState(const bool state)           { _wavedashState = state; }
    void setShieldDropState(const bool state)         { _shieldDropState = state; }
    void setShieldState(const bool state)             { _shieldState = state; }
    void setJumpState(const bool state)               { _jumpState = state; }
    void setAState(const bool state)                  { _AState = state; }
    void setBState(const bool state)                  { _BState = state; }
    void setSmashDIState(const bool state)            { _smashDIState = state; }

    void setShieldDrop(const uint8_t value)           { _shieldDropValue = value; }
    void setTilt(const uint8_t value)                 { _tiltAmount = value; }

    void setModStart(const uint8_t value)             { _xAxis.setModValue(value); _yAxis.setModValue(value); }

    void setRange(const uint8_t value)
    {
        FightingJoystick::setRange(value);
        _xAxis.setRange(value);
        _yAxis.setRange(value);
        _range = value;
    }

private:
    Activator _tiltState;
    Activator _shieldState;
    Activator _shieldDropState;
    Activator _wavedashState;
    Activator _lsDownState;
    Activator _lsUpState;
    Activator _lsLeftState;
    Activator _lsRightState;
    Activator _jumpState;
    Activator _xModState;
    Activator _yModState;
    Activator _AState;
    Activator _BState;
    Activator _smashDIState;

    uint8_t _tiltAmount;
    uint8_t _range;
    uint8_t _shieldDropValue;
    uint8_t _ABTiltValue;

    TemporaryActivator _tiltXActivator;
    TemporaryActivator _tiltYActivator;
    TemporaryActivator _forceWaveland;

    SingleModAxis _xAxis;
    SingleModAxis _yAxis;

    TwoButtonSpamMacro _smashDIMacro;
};

#endif // EIGHTWAYLEFTSTICK_H

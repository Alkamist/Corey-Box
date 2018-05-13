#ifndef EIGHTWAYLEFTSTICK_H
#define EIGHTWAYLEFTSTICK_H

#include "FightingJoystick.h"
#include "SingleModAxis.h"
#include "Signum.h"
#include "TemporaryActivator.h"
#include "TimedActivator.h"
#include "ScaleBipolar.h"
#include "ClampBipolar.h"
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
        _walkTiltActivator.setTime(16);

        // You have to hold down for a certain period of time to get into crouch.
        _inCrouch.setTime(10);

        _killXTilt.setTime(4);
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
        handleBackdashOutOfCrouchFix();
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
        // Tilt reset conditions
        bool walkTilt = _lsUpState.justDeactivated() && (_lsLeftState || _lsRightState);

        _tiltXActivator = xJustLeftDeadZone() || xValue.justCrossedCenter() || _shieldState.justActivated() || _AState.justActivated() || walkTilt;
        _tiltXActivator.process();

        _tiltYActivator = yJustLeftDeadZone() || yValue.justCrossedCenter() || _shieldState.justActivated() || _AState.justActivated();
        _tiltYActivator.process();

        // Tilt conditions
        _walkTiltActivator = walkTilt;
        _walkTiltActivator.process();

        bool passiveYTilt = _lsUpState && !_BState;
        bool allowShieldDrop = _shieldState && _lsDownState && !_tiltState && !_xModState && !_yModState && !_lsUpState;
        bool disableTilt = _wavedashState || (_BState && _shieldState);

        bool tiltXCondition = (_tiltState || _shieldState || _xModState || _yModState || _AState || _walkTiltActivator)
                           && !disableTilt;
        bool tiltYCondition = (_tiltState || _shieldState || _xModState || _yModState || passiveYTilt || _AState)
                           && !disableTilt && !allowShieldDrop;

        // Tilt X
        if (_tiltXActivator && tiltXCondition)
        {
            xValue = scaleBipolar(xValue, _tiltAmount, _range);

            // Don't let the tilt scaling force values into the deadzone unintentionally.
            if (xIsInDeadZone())
            {
                if (xValue < 128) xValue = 128 - getDeadZoneUpperBound() - 1;
                if (xValue > 128) xValue = 128 + getDeadZoneUpperBound() + 1;
            }
        }

        // Tilt Y
        if (_tiltYActivator && tiltYCondition)
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
        bool ABTiltDisabler = _xModState || _yModState || onlyLeft || onlyRight;

        if (AOrB && !ABTiltDisabler)
        {
            xValue = scaleBipolar(xValue, _ABTiltValue);
        }

        FightingJoystick::process();
    }

    void handleBackdashOutOfCrouchFix()
    {
        bool onlyLeft = _lsLeftState && !(_lsUpState || _lsDownState);
        bool onlyRight = _lsRightState && !(_lsUpState || _lsDownState);

        bool backdashFixDisable = (_gameMode != 0) || _AState || _BState || _shieldState || _xModState || _yModState
                               || _jumpState || _wavedashState || _smashDIState || onlyLeft || onlyRight;

        _inCrouch = _lsDownState;
        _inCrouch.process();

        _killXTilt = (_lsLeftState.justActivated() || _lsRightState.justActivated()) && _inCrouch;
        _killXTilt.process();

        if (_killXTilt && !backdashFixDisable)
        {
                yValue = 128 - _range;
                xValue = 128;
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

        _gameMode.endCycle();

        _tiltXActivator.endCycle();
        _tiltYActivator.endCycle();
        _forceWaveland.endCycle();
        _walkTiltActivator.endCycle();
        _killXTilt.endCycle();

        _inCrouch.endCycle();

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

    void setGameMode(const uint8_t value)             { _gameMode = value; }
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

    Control _gameMode;

    uint8_t _tiltAmount;
    uint8_t _range;
    uint8_t _shieldDropValue;
    uint8_t _ABTiltValue;

    TemporaryActivator _tiltXActivator;
    TemporaryActivator _tiltYActivator;
    TemporaryActivator _forceWaveland;
    TemporaryActivator _walkTiltActivator;
    TemporaryActivator _killXTilt;

    TimedActivator _inCrouch;

    SingleModAxis _xAxis;
    SingleModAxis _yAxis;

    TwoButtonSpamMacro _smashDIMacro;
};

#endif // EIGHTWAYLEFTSTICK_H

#ifndef EIGHTWAYLEFTSTICK_H
#define EIGHTWAYLEFTSTICK_H

#include "TemporaryActivator.h"
#include "TimedActivator.h"
#include "TwoButtonControl.h"
#include "TwoButtonSpamMacro.h"
#include "TwoButtonStateTracker.h"

// This class represents the left analog stick used in the eight way controller.
class EightWayLeftStick
{
public:
    EightWayLeftStick()
    : _xValue(128),
      _yValue(128),
      _maxWavelandValue(25),
      _maxWavedashValue(25),
      _shortWavedashValue(35),
      _tiltXValue(40),
      _tiltYValue(40),
      _shieldDropValue(74),
      _ABTiltValue(60),
      _xModValue(25),
      _yModValue(25)
    {
        _tiltXActivator.setTime(12);
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

        _xValue = _xAxis;
        _yValue = _yAxis;

        handleBackdashOutOfCrouchFix();
        handleABAngles();
        handleTilting();
        handleWavelands();
        handleWavedashes();
        handleShieldDropping();
        handleModifiers();
        handleSmashDI();
    }

    // Force perfect wavelands when holding straight left or right.
    void handleWavelands()
    {
        bool straightLeftOrRight = (_lsLeftState || _lsRightState) && !(_lsDownState || _lsUpState);

        _forceWaveland = _shieldState.justActivated();
        _forceWaveland.process();

        if (_shieldState && straightLeftOrRight && _forceWaveland)
            _yValue = 128 - _maxWavelandValue;
    }

    // Force perfect wavedashes when holding straight left or right.
    // Force wavedash down when stick is neutral.
    // Force short wavedash when stick is 45 degrees up.
    void handleWavedashes()
    {
        if (_wavedashState)
        {
            bool straightLeftOrRight = (_lsLeftState || _lsRightState) && !(_lsDownState || _lsUpState);
            bool diagonallyDown = (_lsLeftState || _lsRightState) && !_lsUpState && _lsDownState;
            bool diagonallyUp = (_lsLeftState || _lsRightState) && _lsUpState && !_lsDownState;
            bool stickIsNeutral = !(_lsLeftState || _lsRightState);

            // Max wavedash
            if (straightLeftOrRight)
            {
                _yValue = 128 - _maxWavedashValue;
                _xValue = _xAxis;
            }

            // Normal wavedash
            if (diagonallyDown)
            {
                _yValue.setToMinimum();
                _xValue = _xAxis;
            }

            // Short wavedash
            if (diagonallyUp)
            {
                _yValue.setToMinimum();
                _xValue.setBipolarMagnitude(_shortWavedashValue);
            }

            // Wavedash down
            if (stickIsNeutral)
            {
                _yValue.setToMinimum();
            }
        }
    }

    void handleShieldDropping()
    {
        bool shieldDropCondition = _shieldDropState && _shieldState;
        bool shieldDropDisable = _wavedashState || _tiltState || _xModState || _yModState;

        if (shieldDropCondition && !shieldDropDisable)
        {
            if (_lsDownState && !(_lsLeftState || _lsRightState))
                _yValue = _shieldDropValue;
        }
    }

    void handleTilting()
    {
        // Tilt reset conditions
        bool walkTilt = (_lsLeftState.justActivated() || _lsRightState.justActivated()) && _lsUpState;

        bool xAxisMoved = _lsLeftState.justActivated() || _lsRightState.justActivated();
        _tiltXActivator = (xAxisMoved && _shieldState) || (xAxisMoved && _yModState)
                       || _AState.justActivated() || walkTilt;
        _tiltXActivator.process();

        bool yAxisMoved = _lsDownState.justActivated() || _lsUpState.justActivated();
        _tiltYActivator = (yAxisMoved && _shieldState)
                       || _AState.justActivated() || _lsUpState.justActivated();
        _tiltYActivator.process();

        // Tilt conditions
        _walkTiltActivator = walkTilt;
        _walkTiltActivator.process();

        bool passiveYTilt = _lsUpState && !_BState;
        bool onlyDown = _lsDownState && !(_lsLeftState || _lsRightState || _lsUpState);
        bool allowShieldDrop = _shieldState && onlyDown && !_tiltState && !_xModState && !_yModState && !_lsUpState;
        bool disableTilt = _wavedashState || _BState;

        bool tiltXCondition = (_tiltState || _shieldState || _yModState || _AState || _walkTiltActivator)
                           && !disableTilt;
        bool tiltYCondition = (_tiltState || _shieldState || passiveYTilt || _AState)
                           && !disableTilt && !allowShieldDrop;

        // Tilt X
        if (_tiltXActivator && tiltXCondition)
            _xValue.scaleBipolarMagnitude(_tiltXValue);

        // Tilt Y
        if (_tiltYActivator && tiltYCondition)
            _yValue.scaleBipolarMagnitude(_tiltYValue);
    }

    void handleABAngles()
    {
        bool AOrB = _AState || _BState;

        bool onlyLeft = _lsLeftState && !(_lsUpState || _lsDownState);
        bool onlyRight = _lsRightState && !(_lsUpState || _lsDownState);
        bool ABTiltDisabler = _xModState || _yModState || onlyLeft || onlyRight || _wavedashState || _lButtonState;

        if (AOrB && !ABTiltDisabler)
            _xValue.setBipolarMagnitude(_ABTiltValue);
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
                _yValue.setToMinimum();
                _xValue.setToCenter();
        }
    }

    void handleModifiers()
    {
        if (_xModState)
            _xValue.setBipolarMagnitude(_xModValue);

        if (_yModState)
            _yValue.setBipolarMagnitude(_yModValue);
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
                _xValue = _xAxis;
            else
                _xValue = 128;

            if (_smashDIMacro.getButton2())
                _yValue = _yAxis;
            else
                _yValue = 128;
        }
    }

    void endCycle()
    {
        _xValue.endCycle();
        _yValue.endCycle();

        _tiltState.endCycle();
        _shieldState.endCycle();
        _shieldDropState.endCycle();
        _wavedashState.endCycle();
        _lButtonState.endCycle();
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

    void setLsLeftState(const bool state)        { _lsLeftState = state; _xAxis.setLowState(state); }
    void setLsRightState(const bool state)       { _lsRightState = state; _xAxis.setHighState(state); }
    void setLsDownState(const bool state)        { _lsDownState = state; _yAxis.setLowState(state); }
    void setLsUpState(const bool state)          { _lsUpState = state; _yAxis.setHighState(state); }
    void setXModState(const bool state)          { _xModState = state; }
    void setYModState(const bool state)          { _yModState = state; }
    void setTiltState(const bool state)          { _tiltState = state; }
    void setWavedashState(const bool state)      { _wavedashState = state; }
    void setLButtonState(const bool state)       { _lButtonState = state; }
    void setShieldDropState(const bool state)    { _shieldDropState = state; }
    void setShieldState(const bool state)        { _shieldState = state; }
    void setJumpState(const bool state)          { _jumpState = state; }
    void setAState(const bool state)             { _AState = state; }
    void setBState(const bool state)             { _BState = state; }
    void setSmashDIState(const bool state)       { _smashDIState = state; }

    void setGameMode(const uint8_t value)        { _gameMode = value; }

    void maxWavelandValue(const uint8_t value)   { _maxWavelandValue = value; }
    void maxWavedashValue(const uint8_t value)   { _maxWavedashValue = value; }
    void shortWavedashValue(const uint8_t value) { _shortWavedashValue = value; }
    void setTiltXValue(const uint8_t value)      { _tiltXValue = value; }
    void setTiltYValue(const uint8_t value)      { _tiltYValue = value; }
    void setShieldDropValue(const uint8_t value) { _shieldDropValue = value; }
    void setABTiltValue(const uint8_t value)     { _ABTiltValue = value; }

    void setRange(const uint8_t value)
    {
        _xValue.setRange(value);
        _yValue.setRange(value);
        _xAxis.setRange(value);
        _yAxis.setRange(value);
    }

    const uint8_t getXValue() const              { return _xValue; }
    const uint8_t getYValue() const              { return _yValue; }

private:
    Control _xValue;
    Control _yValue;

    Activator _tiltState;
    Activator _shieldState;
    Activator _shieldDropState;
    Activator _wavedashState;
    Activator _lButtonState;
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

    TwoButtonControl _xAxis;
    TwoButtonControl _yAxis;

    uint8_t _maxWavelandValue;
    uint8_t _maxWavedashValue;
    uint8_t _shortWavedashValue;
    uint8_t _tiltXValue;
    uint8_t _tiltYValue;
    uint8_t _shieldDropValue;
    uint8_t _ABTiltValue;
    uint8_t _xModValue;
    uint8_t _yModValue;

    Control _gameMode;

    TemporaryActivator _tiltXActivator;
    TemporaryActivator _tiltYActivator;
    TemporaryActivator _forceWaveland;
    TemporaryActivator _walkTiltActivator;
    TemporaryActivator _killXTilt;

    TimedActivator _inCrouch;

    TwoButtonSpamMacro _smashDIMacro;
};

#endif // EIGHTWAYLEFTSTICK_H

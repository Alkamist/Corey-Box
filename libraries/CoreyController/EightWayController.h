#ifndef BUTTONONLYCONTROLLER_H
#define BUTTONONLYCONTROLLER_H

#include "GameCubeController.h"
#include "ButtonReader.h"
#include "EightWayLeftStick.h"
#include "ButtonOnlyCStick.h"
#include "WavedashMacro.h"
#include "TwoButtonSpamMacro.h"
#include "TwoButtonStateTracker.h"
#include "ToggleActivator.h"
#include "ShieldManager.h"
#include "JumpManager.h"
#include "DelayedActivator.h"

// This is the main controller class right now. I know it is kind of a
// god class but I'm not sure how to split up this logic.
class EightWayController : public GameCubeController
{
public:
    EightWayController();

    void process();
    void endCycle();

private:
    // Buttons:
    ButtonReader _lsLeftButton;
    ButtonReader _lsRightButton;
    ButtonReader _lsDownButton;
    ButtonReader _lsUpButton;

    ButtonReader _xModButton;
    ButtonReader _yModButton;

    ButtonReader _cLeftButton;
    ButtonReader _cRightButton;
    ButtonReader _cDownButton;
    ButtonReader _cUpButton;

    ButtonReader _aButton;
    ButtonReader _bButton;
    ButtonReader _shortHopButton;
    ButtonReader _fullHopButton;
    ButtonReader _zButton;
    ButtonReader _lButton;
    ButtonReader _rButton;
    ButtonReader _startButton;

    ButtonReader _dPadButton;
    ButtonReader _settingsButton;
    ButtonReader _smashDIButton;

    // Game Mode:
    Control _gameMode;

    // Sticks:
    EightWayLeftStick _leftStick;
    ButtonOnlyCStick _cStick;

    // Macros:
    WavedashMacro _wavedashMacro;
    TwoButtonStateTracker _spamBAActivator;
    TwoButtonSpamMacro _spamBAMacro;

    // Shield:
    ShieldManager _shieldManager;

    // Jump:
    JumpManager _jumpManager;

    // Jump Cancel Grab:
    TemporaryActivator _jumpCancelGrabDelay;
    TemporaryActivator _jumpCancelGrabMinimumPressTime;

    // Accidental Roll Prevention:
    DelayedActivator _rollPreventedR;

    // Extra Activators:
    Activator _trimWavedashDown;
    Activator _trimWavedashUp;
    Activator _trimLsYDown;
    Activator _trimLsYUp;
    Activator _trimLsXDown;
    Activator _trimLsXUp;

    Activator _spamBA;

    // Outputs:
    Activator _aOut;
    Activator _bOut;
    Activator _xOut;
    Activator _yOut;
    Activator _zOut;
    Activator _lOut;
    Activator _rOut;
    Activator _startOut;
    Activator _dLeftOut;
    Activator _dRightOut;
    Activator _dDownOut;
    Activator _dUpOut;
    Control _lAnalogOut;
    Control _rAnalogOut;
    Control _lsXOut;
    Control _lsYOut;
    Control _cXOut;
    Control _cYOut;

    // Process Functions:
    void setTiming();
    void initializeOutputs();
    void preventAccidentalRolls();
    void processActivators();
    void processGameMode();
    void processShieldManager();
    void processJumpManager();
    void processJumpCancelGrab();
    void processSpamBAMacro();
    void processWavedashMacro();
    void processLStick();
    void processCStick();
    void finalizeOutputs();
};



void EightWayController::process()
{
    setTiming();
    initializeOutputs();
    preventAccidentalRolls();
    processActivators();
    processGameMode();
    processShieldManager();
    processJumpManager();
    processJumpCancelGrab();
    processSpamBAMacro();
    processWavedashMacro();
    processLStick();
    processCStick();
    finalizeOutputs();
}

void EightWayController::endCycle()
{
    GameCubeController::endCycle();

    // Buttons:
    _lsLeftButton.endCycle();
    _lsRightButton.endCycle();
    _lsDownButton.endCycle();
    _lsUpButton.endCycle();

    _xModButton.endCycle();
    _yModButton.endCycle();

    _cLeftButton.endCycle();
    _cRightButton.endCycle();
    _cDownButton.endCycle();
    _cUpButton.endCycle();

    _aButton.endCycle();
    _bButton.endCycle();
    _shortHopButton.endCycle();
    _fullHopButton.endCycle();
    _zButton.endCycle();
    _rButton.endCycle();
    _lButton.endCycle();
    _startButton.endCycle();

    _dPadButton.endCycle();
    _settingsButton.endCycle();
    _smashDIButton.endCycle();

    // Game Mode:
    _gameMode.endCycle();

    // Sticks:
    _cStick.endCycle();
    _leftStick.endCycle();

    // Macros:
    _wavedashMacro.endCycle();
    _spamBAMacro.endCycle();

    // Shield:
    _shieldManager.endCycle();

    // Jump:
    _jumpManager.endCycle();

    // Jump Cancel Grab:
    _jumpCancelGrabDelay.endCycle();
    _jumpCancelGrabMinimumPressTime.endCycle();

    // Accidental Roll Prevention:
    _rollPreventedR.endCycle();

    // Extra Activators:
    _trimWavedashDown.endCycle();
    _trimWavedashUp.endCycle();
    _trimLsXDown.endCycle();
    _trimLsXUp.endCycle();
    _trimLsYDown.endCycle();
    _trimLsYUp.endCycle();

    _spamBA.endCycle();

    // Outputs:
    _aOut.endCycle();
    _bOut.endCycle();
    _xOut.endCycle();
    _yOut.endCycle();
    _zOut.endCycle();
    _lOut.endCycle();
    _rOut.endCycle();
    _startOut.endCycle();
    _dLeftOut.endCycle();
    _dRightOut.endCycle();
    _dDownOut.endCycle();
    _dUpOut.endCycle();
    _lAnalogOut.endCycle();
    _rAnalogOut.endCycle();
    _lsXOut.endCycle();
    _lsYOut.endCycle();
    _cXOut.endCycle();
    _cYOut.endCycle();
}

void EightWayController::setTiming()
{
    if (_settingsButton && _zButton)
        HalfFramesElapsed::setConsoleTiming();

    if (_settingsButton && _rButton)
        HalfFramesElapsed::setDolphinTiming();
}

void EightWayController::initializeOutputs()
{
    _aOut = _aButton;
    _bOut = _bButton;
    _xOut = false;
    _yOut = false;
    _zOut = false;
    _lOut = false;
    _rOut = false;
    _rAnalogOut = 0;
    _startOut = _startButton;
    _dLeftOut = false;
    _dRightOut = false;
    _dDownOut = false;
    _dUpOut = false;

    // D-pad Modifier:
    if (_dPadButton)
    {
        _dLeftOut = _lsLeftButton;
        _dRightOut = _lsRightButton;
        _dDownOut = _lsDownButton;
        _dUpOut = _lsUpButton;
    }
}

void EightWayController::preventAccidentalRolls()
{
    _rollPreventedR = _rButton;
    _rollPreventedR.setDelayCondition((_lsLeftButton.justActivated() || _lsRightButton.justActivated())
                                   && !(_lsDownButton || _lsUpButton));
    _rollPreventedR.process();
}

void EightWayController::processActivators()
{
    _trimWavedashDown = _settingsButton && !_dPadButton && _cDownButton;
    _trimWavedashUp = _settingsButton && !_dPadButton && _cUpButton;

    _trimLsYDown = _settingsButton && _dPadButton && _cDownButton;
    _trimLsYUp = _settingsButton && _dPadButton && _cUpButton;
    _trimLsXDown = _settingsButton && _dPadButton && _cLeftButton;
    _trimLsXUp = _settingsButton && _dPadButton && _cRightButton;
}

void EightWayController::processGameMode()
{
    if (_settingsButton && _lsDownButton) _gameMode = 0;
    if (_settingsButton && _lsUpButton) _gameMode = 1;
    if (_settingsButton && _lsLeftButton) _gameMode = 2;
    if (_settingsButton && _lsRightButton) _gameMode = 3;

    if (_gameMode.justChanged())
    {
        // Melee:
        if (_gameMode == 0)
        {
            _leftStick.setGameMode(_gameMode);
            _leftStick.setRange(80);
            _cStick.setRange(80);
            _leftStick.maxWavelandValue(34);
            _leftStick.maxWavedashValue(25);
            _leftStick.shortWavedashValue(35);
            _leftStick.setTiltXValue(49);
            _leftStick.setTiltYValue(40);
            _leftStick.setShieldDropValue(74);
            _leftStick.setABTiltValue(40);
            resetLsXTrim();
            resetLsYTrim();
            _wavedashMacro.setMinimumDelay(3);
            return;
        }
        // Project M:
        if (_gameMode == 1)
        {
            _leftStick.setGameMode(_gameMode);
            _leftStick.setRange(80);
            _cStick.setRange(80);
            _leftStick.maxWavedashValue(31);
            _leftStick.shortWavedashValue(31);
            _leftStick.setTiltXValue(64);
            _leftStick.setTiltYValue(64);
            _leftStick.setShieldDropValue(56);
            _leftStick.setABTiltValue(60);
            resetLsXTrim();
            resetLsYTrim();
            _wavedashMacro.setMinimumDelay(3);
            return;
        }
        // Rivals:
        if (_gameMode == 2)
        {
            _leftStick.setGameMode(_gameMode);
            _leftStick.setRange(100);
            _cStick.setRange(100);
            _leftStick.maxWavedashValue(32);
            _leftStick.shortWavedashValue(32);
            _leftStick.setTiltXValue(61);
            _leftStick.setTiltYValue(61);
            _leftStick.setShieldDropValue(28);
            _leftStick.setABTiltValue(60);
            resetLsXTrim();
            resetLsYTrim();
            trimLsYDown();
            _wavedashMacro.setMinimumDelay(3);
            return;
        }
        // Icons:
        if (_gameMode == 3)
        {
            _leftStick.setGameMode(_gameMode);
            _leftStick.setRange(100);
            _cStick.setRange(100);
            _leftStick.maxWavedashValue(43);
            _leftStick.shortWavedashValue(43);
            _leftStick.setTiltXValue(61);
            _leftStick.setTiltYValue(61);
            _leftStick.setShieldDropValue(28);
            _leftStick.setABTiltValue(60);
            resetLsXTrim();
            resetLsYTrim();
            trimLsYDown();
            _wavedashMacro.setMinimumDelay(2);
            return;
        }
    }
}

void EightWayController::processShieldManager()
{
    _shieldManager.setActivator(_rollPreventedR);
    _shieldManager.setToggleState(_cUpButton);
    _shieldManager.process();

    if (!_settingsButton)
    {
        _rOut = _shieldManager.getHardShieldState();
        _rAnalogOut = _shieldManager.getLightShieldOutput();
    }
}

void EightWayController::processJumpManager()
{
    _jumpManager.setShortHopState(_shortHopButton);
    _jumpManager.setFullHopState(_fullHopButton);
    _jumpManager.process();

    _xOut = _jumpManager.getXState();
    _yOut = _jumpManager.getYState();
}

void EightWayController::processJumpCancelGrab()
{
    bool onlyLeft = _lsLeftButton && !(_lsUpButton || _lsDownButton);
    bool onlyRight = _lsRightButton && !(_lsUpButton || _lsDownButton);

    bool doJumpCancelGrab = (onlyLeft || onlyRight) && _zButton;

    _jumpCancelGrabDelay = _zButton.justActivated() && doJumpCancelGrab;
    _jumpCancelGrabDelay.process();

    _jumpCancelGrabMinimumPressTime = _jumpCancelGrabDelay.justDeactivated();
    _jumpCancelGrabMinimumPressTime.process();

    if (_jumpCancelGrabDelay)
    {
        _xOut = true;
    }
    else
    {
        _zOut = _zButton || _jumpCancelGrabMinimumPressTime;
    }
}

void EightWayController::processSpamBAMacro()
{
    _spamBAActivator.setState1(_bButton);
    _spamBAActivator.setState2(_aButton);
    _spamBAActivator.process();

    _spamBA = _spamBAActivator.state1WasFirst() && _spamBAActivator.getState2();

    _spamBAMacro = _spamBA;
    _spamBAMacro.process();

    if (_spamBAMacro.isRunning())
    {
        _bOut = _spamBAMacro.getButton1();

        if (_spamBA && _cDownButton) _aOut = _spamBAMacro.getButton2();
    }
}

void EightWayController::processWavedashMacro()
{
    _wavedashMacro = _lButton && !_settingsButton;

    if (_trimWavedashDown.justActivated()) _wavedashMacro.trimDown();
    if (_trimWavedashUp.justActivated())   _wavedashMacro.trimUp();

    _wavedashMacro.process();

    if (_wavedashMacro.getJump().isRunning())
        _yOut = _wavedashMacro.getJump();

    if (_wavedashMacro.getR().isRunning())
        _rOut = _wavedashMacro.getR();

    _lOut = _wavedashMacro.getL();

    if (_settingsButton)
    {
        _lOut = _lButton;
        _rOut = _rButton;
    }
}

void EightWayController::processLStick()
{
    bool disableDirections = _dPadButton || _settingsButton;

    _leftStick.setLsLeftState(_lsLeftButton && !disableDirections);
    _leftStick.setLsRightState(_lsRightButton && !disableDirections);
    _leftStick.setLsDownState(_lsDownButton && !disableDirections);
    _leftStick.setLsUpState(_lsUpButton && !disableDirections);
    _leftStick.setShieldDropState(!disableDirections);
    _leftStick.setXModState(_xModButton);
    _leftStick.setYModState(_yModButton);
    _leftStick.setTiltState(false);
    _leftStick.setWavedashState(_wavedashMacro.getR().isRunning());
    _leftStick.setLButtonState(_lButton);
    _leftStick.setShieldState(_shieldManager);
    _leftStick.setJumpState(_shortHopButton || _fullHopButton);
    _leftStick.setAState(_aButton);
    _leftStick.setBState(_bButton);
    _leftStick.setSmashDIState(_smashDIButton);

    if (_trimLsXDown.justActivated()) trimLsXDown();
    if (_trimLsXUp.justActivated())   trimLsXUp();

    if (_trimLsYDown.justActivated()) trimLsYDown();
    if (_trimLsYUp.justActivated())   trimLsYUp();

    _leftStick.process();

    _lsXOut = _leftStick.getXValue();
    _lsYOut = _leftStick.getYValue();
}

void EightWayController::processCStick()
{
    bool disableCDirections = _settingsButton;

    _cStick.setCLeftState(_cLeftButton && !disableCDirections);
    _cStick.setCRightState(_cRightButton && !disableCDirections);
    // Disable cDown while the Spam BA macro is active.
    _cStick.setCDownState(_cDownButton && !_spamBA && !disableCDirections);
    _cStick.setCUpState(_cUpButton && !disableCDirections);
    _cStick.setLsDownState(_lsDownButton);
    _cStick.setLsUpState(_lsUpButton);
    _cStick.setTiltState(_lsDownButton || _lsUpButton);

    _cStick.process();

    _cXOut = _cStick.getXValue();
    _cYOut = _cStick.getYValue();

    // This allows cUp to be pressed while in shield to toggle light shield.
    bool lightShieldJumpClear = _shortHopButton || _fullHopButton;
    if (_rButton && _cUpButton && !lightShieldJumpClear)
        _cYOut = 128;
}

void EightWayController::finalizeOutputs()
{
    // Push A at the same time as any c-stick button if one of the mods is pressed.
    // This allows for charged smashes since you can't get them with the stick and A.
    bool anyCDirection = _cLeftButton || _cDownButton || _cRightButton || _cUpButton;
    bool eitherMod = _xModButton || _yModButton;

    a = _aOut || (eitherMod && anyCDirection);
    b = _bOut;
    x = _xOut;
    y = _yOut;
    z = _zOut;
    start = _startOut;
    dLeft = _dLeftOut;
    dRight = _dRightOut;
    dDown = _dDownOut;
    dUp = _dUpOut;
    lsX = _lsXOut + getLsXOffset();
    lsY = _lsYOut + getLsYOffset();
    cX = _cXOut;
    cY = _cYOut;

    // Use analog shield values for Icons.
    if (_gameMode != 3)
    {
        l = _lOut;
        r = _rOut;
        lAnalog = _lAnalogOut;
        rAnalog = _rAnalogOut;
    }
    else
    {
        l = false;
        r = false;
        lAnalog = _lOut * 120;
        rAnalog = _rOut * 120;
    }
}

// Don't use pin 6 or 26 for buttons.
EightWayController::EightWayController()
: // Buttons:
  _lsLeftButton(9),
  _lsRightButton(8),
  _lsDownButton(11),
  _lsUpButton(10),
  _xModButton(24),
  _yModButton(22),
  _cLeftButton(2),
  _cRightButton(39),
  _cDownButton(3),
  _cUpButton(38),
  _aButton(27),
  _bButton(40),
  _shortHopButton(19),
  _fullHopButton(23),
  _zButton(20),
  _lButton(18),
  _rButton(21),
  _startButton(5),
  _dPadButton(25),
  _settingsButton(7),
  _smashDIButton(4)
{
    ButtonReader::setUseBounce(true);

    _jumpCancelGrabDelay.setTime(3);
    _jumpCancelGrabMinimumPressTime.setTime(3);

    _rollPreventedR.setDelayTime(8);
}

#endif // BUTTONONLYCONTROLLER_H
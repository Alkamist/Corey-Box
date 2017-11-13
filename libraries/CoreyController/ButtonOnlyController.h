#ifndef BUTTONONLYCONTROLLER_H
#define BUTTONONLYCONTROLLER_H

#include "GameCubeController.h"
#include "ButtonReader.h"
#include "ButtonOnlyLeftStick.h"
#include "ButtonOnlyCStick.h"
#include "WavedashMacro.h"
#include "TwoButtonSpamMacro.h"
#include "TwoButtonStateTracker.h"
#include "ToggleActivator.h"
#include "ShieldManager.h"
#include "JumpManager.h"

// This is the main controller class right now. I know it is kind of a
// god class but I'm not sure how to split up this logic.
class ButtonOnlyController : public GameCubeController
{
public:
    ButtonOnlyController();

    void process();
    void endCycle();

private:
    // Buttons:
    ButtonReader _tiltButton;
    ButtonReader _unUsedButton;

    ButtonReader _lsLeftButton;
    ButtonReader _lsRightButton;
    ButtonReader _lsDownButton;
    ButtonReader _lsUpButton;

    ButtonReader _xMod1Button;
    ButtonReader _xMod2Button;
    ButtonReader _yMod1Button;
    ButtonReader _yMod2Button;

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
    ButtonReader _wavedashTrimButton;
    ButtonReader _settingsButton;
    ButtonReader _dUpButton;

    // Game Mode:
    Control _gameMode;

    // Sticks:
    ButtonOnlyLeftStick _leftStick;
    ButtonOnlyCStick _cStick;

    // Macros:
    WavedashMacro _wavedashMacro;
    TwoButtonStateTracker _spamBAActivator;
    TwoButtonSpamMacro _spamBAMacro;

    // Shield:
    ShieldManager _shieldManager;

    // Jump:
    JumpManager _jumpManager;

    // Extra Activators:
    Activator _trimWavedashDown;
    Activator _trimWavedashUp;
    Activator _trimLsYDown;
    Activator _trimLsYUp;
    Activator _trimLsXDown;
    Activator _trimLsXUp;

    Activator _spamBA;

    Activator _disableMacros;
    ToggleActivator _macrosAreOn;

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
    void processActivators();
    void processGameMode();
    void processShieldManager();
    void processJumpManager();
    void processSpamBAMacro();
    void processWavedashMacro();
    void processLStick();
    void processCStick();
    void finalizeOutputs();
};



void ButtonOnlyController::process()
{
    setTiming();
    initializeOutputs();
    processActivators();
    processGameMode();
    processShieldManager();
    processJumpManager();
    processSpamBAMacro();
    processWavedashMacro();
    processLStick();
    processCStick();
    finalizeOutputs();
}

void ButtonOnlyController::endCycle()
{
    GameCubeController::endCycle();

    // Buttons:
    _tiltButton.endCycle();
    _unUsedButton.endCycle();

    _lsLeftButton.endCycle();
    _lsRightButton.endCycle();
    _lsDownButton.endCycle();
    _lsUpButton.endCycle();

    _xMod1Button.endCycle();
    _xMod2Button.endCycle();
    _yMod1Button.endCycle();
    _yMod2Button.endCycle();

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
    _wavedashTrimButton.endCycle();
    _settingsButton.endCycle();
    _dUpButton.endCycle();

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

    // Extra Activators:
    _trimWavedashDown.endCycle();
    _trimWavedashUp.endCycle();
    _trimLsXDown.endCycle();
    _trimLsXUp.endCycle();
    _trimLsYDown.endCycle();
    _trimLsYUp.endCycle();

    _spamBA.endCycle();

    _disableMacros.endCycle();
    _macrosAreOn.endCycle();

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

void ButtonOnlyController::setTiming()
{
    if (_settingsButton && _zButton)
        HalfFramesElapsed::setConsoleTiming();

    if (_settingsButton && _rButton)
        HalfFramesElapsed::setDolphinTiming();
}

void ButtonOnlyController::initializeOutputs()
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
    _dUpOut = _dUpButton;

    // D-pad Modifier:
    if (_unUsedButton)
    {
        _dLeftOut = _lsLeftButton;
        _dRightOut = _lsRightButton;
        _dDownOut = _lsDownButton;
        _dUpOut = _lsUpButton || _dUpButton;
    }

    if (!_macrosAreOn)
    {
        _xOut = _fullHopButton;
        _yOut = _shortHopButton;
        _lOut = _lButton;
    }

    if (!_settingsButton) _zOut = _zButton;
}

void ButtonOnlyController::processActivators()
{
    bool allMods = _xMod1Button && _xMod2Button && _yMod1Button && _yMod2Button;
    bool macrosOff = allMods && _unUsedButton;
    bool macrosOn = allMods && _dUpButton;

    if (allMods) _dUpOut = false;

    if (_macrosAreOn) _disableMacros = macrosOff;
    if (!_macrosAreOn) _disableMacros = macrosOn;

    _macrosAreOn = _disableMacros.justActivated();
    _macrosAreOn.process();

    ButtonReader::setUseBounce(_macrosAreOn);

    _trimWavedashDown = _wavedashTrimButton && _cDownButton;
    _trimWavedashUp = _wavedashTrimButton && _cUpButton;

    _trimLsYDown = _settingsButton && _cDownButton;
    _trimLsYUp = _settingsButton && _cUpButton;
    _trimLsXDown = _settingsButton && _cLeftButton;
    _trimLsXUp = _settingsButton && _cRightButton;
}

void ButtonOnlyController::processGameMode()
{
    if (_settingsButton && _lsDownButton) _gameMode = 0;
    if (_settingsButton && _lsUpButton) _gameMode = 1;
    if (_settingsButton && _lsLeftButton) _gameMode = 2;

    if (_gameMode.justChanged())
    {
        // Melee:
        if (_gameMode == 0)
        {
            _leftStick.setRange(80);
            _cStick.setRange(80);
            _leftStick.setModStart(24);
            _leftStick.setShieldDrop(74);
            _leftStick.setDeadZoneUpperBound(22);
            _leftStick.setTilt(49);
            resetLsXTrim();
            resetLsYTrim();
            return;
        }
        // Project M:
        if (_gameMode == 1)
        {
            _leftStick.setRange(80);
            _cStick.setRange(80);
            _leftStick.setModStart(31);
            _leftStick.setShieldDrop(56);
            _leftStick.setDeadZoneUpperBound(30);
            _leftStick.setTilt(64);
            resetLsXTrim();
            resetLsYTrim();
            return;
        }
        // Rivals:
        if (_gameMode == 2)
        {
            _leftStick.setRange(100);
            _cStick.setRange(100);
            _leftStick.setModStart(32);
            _leftStick.setShieldDrop(28);
            _leftStick.setDeadZoneUpperBound(27);
            _leftStick.setTilt(61);
            resetLsXTrim();
            resetLsYTrim();
            trimLsYDown();
            return;
        }
    }
}

void ButtonOnlyController::processShieldManager()
{
    _shieldManager.setActivator(_rButton);
    _shieldManager.setToggleState(_cUpButton);
    _shieldManager.process();

    if (!_settingsButton)
    {
        _rOut = _shieldManager.getHardShieldState();
        _rAnalogOut = _shieldManager.getLightShieldOutput();
    }
}

void ButtonOnlyController::processJumpManager()
{
    if (_macrosAreOn)
    {
        _jumpManager.setShortHopState(_shortHopButton);
        _jumpManager.setFullHopState(_fullHopButton);
        _jumpManager.process();

        _xOut = _jumpManager.getXState();
        _yOut = _jumpManager.getYState();
    }
}

void ButtonOnlyController::processSpamBAMacro()
{
    if (_macrosAreOn)
    {
        _spamBAActivator.setState1(_bButton);
        _spamBAActivator.setState2(_aButton);
        _spamBAActivator.process();

        _spamBA = _spamBAActivator.state1WasFirst() && _spamBAActivator.getState2();

        _spamBAMacro = _spamBA && _macrosAreOn;
        _spamBAMacro.process();

        if (_spamBAMacro.isRunning())
        {
            _bOut = _spamBAMacro.getButton1();

            if (_spamBA && _cDownButton) _aOut = _spamBAMacro.getButton2();
        }
    }
}

void ButtonOnlyController::processWavedashMacro()
{
    _wavedashMacro = _lButton && !_wavedashTrimButton && _macrosAreOn;

    if (_trimWavedashDown.justActivated()) _wavedashMacro.trimDown();
    if (_trimWavedashUp.justActivated())   _wavedashMacro.trimUp();

    _wavedashMacro.process();

    if (_macrosAreOn)
    {
        if (_wavedashMacro.getJump().isRunning())
            _yOut = _wavedashMacro.getJump();

        if (_wavedashMacro.getR().isRunning())
            _rOut = _wavedashMacro.getR();

        _lOut = _wavedashMacro.getL();
    }

    if (_wavedashTrimButton) _lOut = _lButton;
}

void ButtonOnlyController::processLStick()
{
    bool disableDirections = _unUsedButton || _settingsButton;

    _leftStick.setLsLeftState(_lsLeftButton && !disableDirections);
    _leftStick.setLsRightState(_lsRightButton && !disableDirections);
    _leftStick.setLsDownState(_lsDownButton && !disableDirections);
    _leftStick.setLsUpState(_lsUpButton && !disableDirections);
    _leftStick.setShieldDropState(_lsDownButton && !_lsLeftButton && !_lsRightButton && !_tiltButton && !disableDirections);

    _leftStick.setXMod1State(_xMod1Button);
    _leftStick.setXMod2State(_xMod2Button);
    _leftStick.setYMod1State(_yMod1Button);
    _leftStick.setYMod2State(_yMod2Button);
    _leftStick.setTiltState(_tiltButton);
    _leftStick.setTiltTempDisableState(_lOut);
    _leftStick.setWavedashState(_wavedashMacro.getR().isRunning());
    _leftStick.setShieldState(_shieldManager);
    _leftStick.setUseMacros(_macrosAreOn);
    _leftStick.setBackdashFixDisableState(_wavedashMacro.isRunning() || _lOut || _yOut || _xOut
                                       || _zOut || _aOut || _bOut || _rOut || (_gameMode != 0));

    if (_trimLsXDown.justActivated()) trimLsXDown();
    if (_trimLsXUp.justActivated())   trimLsXUp();

    if (_trimLsYDown.justActivated()) trimLsYDown();
    if (_trimLsYUp.justActivated())   trimLsYUp();

    _leftStick.process();

    _lsXOut = _leftStick.xValue;
    _lsYOut = _leftStick.yValue;
}

void ButtonOnlyController::processCStick()
{
    bool disableCDirections = _wavedashTrimButton || _settingsButton;

    _cStick.setCLeftState(_cLeftButton && !disableCDirections);
    _cStick.setCRightState(_cRightButton && !disableCDirections);
    // Disable cDown while the Spam BA macro is active.
    _cStick.setCDownState(_cDownButton && !_spamBA && !disableCDirections);
    _cStick.setCUpState(_cUpButton && !disableCDirections);
    _cStick.setLsDownState(_lsDownButton);
    _cStick.setLsUpState(_lsUpButton);
    _cStick.setTiltState(_tiltButton);

    _cStick.process();

    _cXOut = _cStick.xValue;
    _cYOut = _cStick.yValue;

    // This allows cUp to be pressed while in shield to toggle light shield.
    bool lightShieldJumpClear = _shortHopButton || _fullHopButton;
    if (_rButton && _cUpButton && !lightShieldJumpClear)
        _cYOut = 128;
}

void ButtonOnlyController::finalizeOutputs()
{
    a = _aOut;
    b = _bOut;
    x = _xOut;
    y = _yOut;
    z = _zOut;
    l = _lOut;
    r = _rOut;
    start = _startOut;
    dLeft = _dLeftOut;
    dRight = _dRightOut;
    dDown = _dDownOut;
    dUp = _dUpOut;
    lAnalog = _lAnalogOut;
    rAnalog = _rAnalogOut;
    lsX = _lsXOut + getLsXOffset();
    lsY = _lsYOut + getLsYOffset();
    cX = _cXOut;
    cY = _cYOut;
}

// Don't use pin 6 or 26 for buttons.
ButtonOnlyController::ButtonOnlyController()
: // Buttons:
  _tiltButton(27),
  _unUsedButton(11),
  _lsLeftButton(2),
  _lsRightButton(5),
  _lsDownButton(3),
  _lsUpButton(4),
  _xMod1Button(10),
  _xMod2Button(9),
  _yMod1Button(8),
  _yMod2Button(7),
  _cLeftButton(39),
  _cRightButton(19),
  _cDownButton(40),
  _cUpButton(20),
  _aButton(38),
  _bButton(18),
  _shortHopButton(23),
  _fullHopButton(22),
  _zButton(25),
  _lButton(21),
  _rButton(24),
  _startButton(13),
  _wavedashTrimButton(14),
  _settingsButton(15),
  _dUpButton(12),
  _macrosAreOn(false)
{}

#endif // BUTTONONLYCONTROLLER_H
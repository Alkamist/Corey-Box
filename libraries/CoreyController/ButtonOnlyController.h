#ifndef BUTTONONLYCONTROLLER_H
#define BUTTONONLYCONTROLLER_H

#include "GameCubeController.h"
#include "BounceButton.h"
#include "NormalButton.h"
#include "ButtonOnlyLeftStick.h"
#include "ButtonOnlyCStick.h"
#include "WavedashMacro.h"
#include "TwoButtonSpamMacro.h"
#include "TwoButtonStateTracker.h"
#include "ToggleActivator.h"
#include "TemporaryActivator.h"
#include "TimedActivator.h"
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
    BounceButton _tiltButton;
    BounceButton _unUsedButton;

    BounceButton _lsLeftButton;
    BounceButton _lsRightButton;
    BounceButton _lsDownButton;
    BounceButton _lsUpButton;

    BounceButton _xMod1Button;
    BounceButton _xMod2Button;
    BounceButton _yMod1Button;
    BounceButton _yMod2Button;

    BounceButton _cLeftButton;
    BounceButton _cRightButton;
    BounceButton _cDownButton;
    BounceButton _cUpButton;

    BounceButton _aButton;
    BounceButton _bButton;
    BounceButton _shortHopButton;
    BounceButton _fullHopButton;
    BounceButton _zButton;
    BounceButton _lButton;
    BounceButton _rButton;
    BounceButton _startButton;
    BounceButton _wavedashTrimButton;
    BounceButton _settingsButton;
    BounceButton _dUpButton;

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
    TemporaryActivator _lightShieldJumpClear;

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

    TimedActivator _disableMacros;
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
    _lightShieldJumpClear.endCycle();

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
        FramesElapsed::setConsoleTiming();

    if (_settingsButton && _rButton)
        FramesElapsed::setDolphinTiming();
}

void ButtonOnlyController::initializeOutputs()
{
    _aOut = _aButton;
    _bOut = _bButton;
    _xOut = false;
    _yOut = false;
    _zOut = _zButton;
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
        _dLeftOut = _yMod1Button;
        _dRightOut = _xMod2Button;
        _dDownOut = _xMod1Button;
        _dUpOut = _yMod2Button || _dUpButton;
    }
}

void ButtonOnlyController::processActivators()
{
    _disableMacros = _wavedashTrimButton && _settingsButton;
    _disableMacros.process();

    _macrosAreOn = _disableMacros.justActivated();
    _macrosAreOn.process();

    _trimWavedashDown = _wavedashTrimButton && _cDownButton;
    _trimWavedashUp = _wavedashTrimButton && _cUpButton;

    _trimLsYDown = _settingsButton && _cDownButton;
    _trimLsYUp = _settingsButton && _cUpButton;
    _trimLsXDown = _settingsButton && _cLeftButton;
    _trimLsXUp = _settingsButton && _cRightButton;
}

void ButtonOnlyController::processGameMode()
{
    if (_settingsButton && _yMod2Button) _gameMode = 0;
    if (_settingsButton && _yMod1Button) _gameMode = 1;
    if (_settingsButton && _xMod2Button) _gameMode = 2;

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
    _shieldManager.setLightShieldState(_cUpButton && _rButton);
    _shieldManager.process();

    _rOut = _shieldManager.getHardShieldState();
    _rAnalogOut = _shieldManager.getLightShieldOutput();
}

void ButtonOnlyController::processJumpManager()
{
    _jumpManager.setShortHopState(_shortHopButton);
    _jumpManager.setFullHopState(_fullHopButton);
    _jumpManager.process();

    _xOut = _jumpManager.getXState();
    _yOut = _jumpManager.getYState();
}

void ButtonOnlyController::processSpamBAMacro()
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
    _leftStick.setLsLeftState(_lsLeftButton);
    _leftStick.setLsRightState(_lsRightButton);
    _leftStick.setLsDownState(_lsDownButton);
    _leftStick.setLsUpState(_lsUpButton);
    _leftStick.setXMod1State(_xMod1Button);
    _leftStick.setXMod2State(_xMod2Button);
    _leftStick.setYMod1State(_yMod1Button);
    _leftStick.setYMod2State(_yMod2Button);
    _leftStick.setTiltState(_tiltButton);
    _leftStick.setTiltTempDisableState(l);
    _leftStick.setWavedashState(_wavedashMacro.getR().isRunning());
    _leftStick.setShieldDropState(_lsDownButton && !_lsLeftButton && !_lsRightButton && !_tiltButton);
    _leftStick.setShieldState(_shieldManager);
    _leftStick.setBackdashFixDisableState(_wavedashMacro.isRunning() || _lOut || _rOut || _yOut
                                       || _xOut || _zOut || _aOut || _bOut || (_gameMode != 0));

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
    _cStick.setCLeftState(_cLeftButton);
    _cStick.setCRightState(_cRightButton);
    // Disable cDown while the Spam BA macro is active.
    _cStick.setCDownState(_cDownButton && !_spamBA);
    _cStick.setCUpState(_cUpButton);
    _cStick.setLsDownState(_lsDownButton);
    _cStick.setLsUpState(_lsUpButton);
    _cStick.setTiltState(_tiltButton);

    _cStick.process();

    _cXOut = _cStick.xValue;
    _cYOut = _cStick.yValue;

    // This allows cUp to be pressed while in shield to toggle light shield.
    // Pushing jump will disable this lock for 4 frames.
    _lightShieldJumpClear = _shortHopButton || _fullHopButton;
    _lightShieldJumpClear.process();
    if (_rButton && _cUpButton && !_lightShieldJumpClear)
        _cYOut = 128;
}

void ButtonOnlyController::finalizeOutputs()
{
    if (!_macrosAreOn)
    {
        _xOut = _fullHopButton;
        _yOut = _shortHopButton;
        _lOut = _lButton;
    }

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
  _macrosAreOn(true)
{
    _disableMacros.setTime(120);
    _lightShieldJumpClear.setTime(4);
}

#endif // BUTTONONLYCONTROLLER_H
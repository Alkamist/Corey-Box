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
#include "TemporaryActivator.h"
#include "TimedActivator.h"
#include "Frames.h"
#include "ShieldManager.h"

// This is the main controller class right now. I know it is kind of a
// god class but I'm not sure how to split up this logic.
class ButtonOnlyController : public GameCubeController
{
public:
    ButtonOnlyController();

    void process();
    void endCycle();

    ButtonOnlyLeftStick& getLeftStick() { return _leftStick; }

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
    ButtonReader _jumpButton;
    ButtonReader _zButton;
    ButtonReader _lButton;
    ButtonReader _rButton;
    ButtonReader _startButton;
    ButtonReader _wavedashTrimButton;
    ButtonReader _settingsButton;
    ButtonReader _disableMacrosButton;
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
    TemporaryActivator _lightShieldJumpClear;

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

    // Process Functions:
    void initializeOutputs();
    void processActivators();
    void processGameMode();
    void processShieldManager();
    void processSpamBAMacro();
    void processWavedashMacro();
    void processLStick();
    void processCStick();
};



void ButtonOnlyController::process()
{
    initializeOutputs();
    processActivators();
    processGameMode();
    processShieldManager();
    processSpamBAMacro();
    processWavedashMacro();
    processLStick();
    processCStick();
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
    _jumpButton.endCycle();
    _zButton.endCycle();
    _rButton.endCycle();
    _lButton.endCycle();
    _startButton.endCycle();
    _wavedashTrimButton.endCycle();
    _settingsButton.endCycle();
    _disableMacrosButton.endCycle();
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
}

void ButtonOnlyController::initializeOutputs()
{
    a = _aButton;
    b = _bButton;
    x = false;
    y = _jumpButton;
    z = _zButton;
    l = _lButton;
    r = _rButton;
    rAnalog = 0;
    start = _startButton;
    dLeft = false;
    dRight = false;
    dDown = false;
    dUp = _dUpButton;

    // D-pad Modifier:
    if (_unUsedButton)
    {
        dLeft = _yMod2Button;
        dRight = _xMod2Button;
        dDown = _xMod1Button;
        dUp = _yMod1Button || _dUpButton;
    }
}

void ButtonOnlyController::processActivators()
{
    _disableMacros = _disableMacrosButton;
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
    if (_settingsButton && _yMod1Button) _gameMode = 0;
    if (_settingsButton && _yMod2Button) _gameMode = 1;
    if (_settingsButton && _xMod2Button) _gameMode = 2;

    if (_gameMode.justChanged())
    {
        // Melee:
        if (_gameMode == 0)
        {
            setAnalogRange(80);
            _leftStick.resetMods();
            _leftStick.resetShieldDrop();
            _leftStick.setDeadZoneUpperBound(36);
            _leftStick.resetTilt();
            resetLsXTrim();
            resetLsYTrim();
            return;
        }
        // Project M:
        if (_gameMode == 1)
        {
            setAnalogRange(80);
            _leftStick.setModStart(50);
            _leftStick.setShieldDrop(13);
            _leftStick.setDeadZoneUpperBound(48);
            _leftStick.setTilt(100);
            resetLsXTrim();
            resetLsYTrim();
            return;
        }
        // Rivals:
        if (_gameMode == 2)
        {
            setAnalogRange(100);
            _leftStick.resetMods();
            _leftStick.setShieldDrop(0);
            _leftStick.setDeadZoneUpperBound(36);
            _leftStick.resetTilt();
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

    r = _shieldManager.getHardShieldState();
    rAnalog = _shieldManager.getLightShieldOutput();
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
        b = _spamBAMacro.getButton1();

        if (_spamBA && _cDownButton) a = _spamBAMacro.getButton2();
    }
}

void ButtonOnlyController::processWavedashMacro()
{
    _wavedashMacro = _lButton && !_wavedashTrimButton && _macrosAreOn;

    if (_trimWavedashDown.justActivated()) _wavedashMacro.trimDown();
    if (_trimWavedashUp.justActivated())   _wavedashMacro.trimUp();

    _wavedashMacro.process();

    if (_wavedashTrimButton)
    {
        y = false;
        x = _jumpButton;
    }

    if (_macrosAreOn && !_wavedashTrimButton)
    {
        if (_wavedashMacro.getJump().isRunning())
            y = _wavedashMacro.getJump();

        if (_wavedashMacro.getR().isRunning())
            r = _wavedashMacro.getR();

        l = _wavedashMacro.getL();
    }
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
    _leftStick.setWavedashState(_wavedashMacro.getL());
    _leftStick.setShieldDropState(_lsDownButton && !_lsLeftButton && !_lsRightButton && !_tiltButton);
    _leftStick.setShieldState(_shieldManager);
    _leftStick.setBackdashFixDisableState(_wavedashMacro.isRunning() || l || r || y || x || z || a || b);

    if (_trimLsXDown.justActivated()) trimLsXDown();
    if (_trimLsXUp.justActivated())   trimLsXUp();

    if (_trimLsYDown.justActivated()) trimLsYDown();
    if (_trimLsYUp.justActivated())   trimLsYUp();

    _leftStick.process();

    lsX = _leftStick.xValue;
    lsY = _leftStick.yValue;
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

    cX = _cStick.xValue;
    cY = _cStick.yValue;

    // This allows cUp to be pressed while in shield to toggle light shield.
    // Pushing jump will disable this lock for 4 frames.
    _lightShieldJumpClear = _jumpButton;
    _lightShieldJumpClear.process();
    if (_rButton && _cUpButton && !_lightShieldJumpClear)
        cY = 128;
}

// Don't use pin 6 or 26 for buttons.
ButtonOnlyController::ButtonOnlyController()
: // Buttons:
  _tiltButton(27),
  _unUsedButton(9),
  _lsLeftButton(0),
  _lsRightButton(3),
  _lsDownButton(1),
  _lsUpButton(2),
  _xMod1Button(8),
  _xMod2Button(7),
  _yMod1Button(4),
  _yMod2Button(5),
  _cLeftButton(38),
  _cRightButton(20),
  _cDownButton(39),
  _cUpButton(21),
  _aButton(18),
  _bButton(19),
  _jumpButton(23),
  _zButton(24),
  _lButton(22),
  _rButton(25),
  _startButton(11),
  _wavedashTrimButton(12),
  _settingsButton(14),
  _disableMacrosButton(13),
  _dUpButton(10),
  _macrosAreOn(true)
{
    _disableMacros.setTime(2000);
    _lightShieldJumpClear.setTime(frames(4));
}

#endif // BUTTONONLYCONTROLLER_H

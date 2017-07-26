#ifndef HYBRIDCONTROLLER_H
#define HYBRIDCONTROLLER_H

#include "GameCubeController.h"
#include "GameCubeControllerReader.h"
#include "ButtonReader.h"
#include "ButtonOnlyLeftStick.h"
#include "ButtonOnlyCStick.h"
#include "ShieldManager.h"

// This is the main controller class right now. I know it is kind of a
// god class but I'm not sure how to split up this logic.
class HybridController : public GameCubeController
{
public:
    HybridController();

    void process();
    void endCycle();

private:
    // GameCube Controller Input:
    GameCubeControllerReader _controllerReader;

    // Buttons:
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
    ButtonReader _settingsButton;

    // Game Mode:
    Control _gameMode;

    // Sticks:
    AnalogLeftStick _leftStick;
    ButtonOnlyCStick _cStick;

    // Shield:
    ShieldManager _shieldManager;

    // Extra Activators:
    Activator _trimLsYDown;
    Activator _trimLsYUp;
    Activator _trimLsXDown;
    Activator _trimLsXUp;

    // Process Functions:
    void initializeOutputs();
    void processActivators();
    void processGameMode();
    void processShieldManager();
    void processLStick();
    void processCStick();
};



void HybridController::process()
{
    _controllerReader.process();

    initializeOutputs();
    processActivators();
    processGameMode();
    processShieldManager();
    processLStick();
    processCStick();
}

void HybridController::endCycle()
{
    GameCubeController::endCycle();

    // Buttons:
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
    _settingsButton.endCycle();

    // Game Mode:
    _gameMode.endCycle();

    // Sticks:
    _cStick.endCycle();
    _leftStick.endCycle();

    // Shield:
    _shieldManager.endCycle();

    // Extra Activators:
    _trimLsXDown.endCycle();
    _trimLsXUp.endCycle();
    _trimLsYDown.endCycle();
    _trimLsYUp.endCycle();
}

void HybridController::initializeOutputs()
{
    a = _aButton;
    b = _bButton;
    x = false;
    y = _jumpButton;
    z = _zButton;
    l = _lButton;
    r = _rButton;
    rAnalog = 0;
    start = _controllerReader.start;
    dLeft = _controllerReader.dLeft;
    dRight = _controllerReader.dRight;
    dDown = _controllerReader.dDown;
    dUp = _controllerReader.dUp;
}

void HybridController::processActivators()
{
    _trimLsYDown = _settingsButton && _cDownButton;
    _trimLsYUp = _settingsButton && _cUpButton;
    _trimLsXDown = _settingsButton && _cLeftButton;
    _trimLsXUp = _settingsButton && _cRightButton;
}

void HybridController::processGameMode()
{
    if (_settingsButton && _controllerReader.dUp) _gameMode = 0;
    if (_settingsButton && _controllerReader.dLeft) _gameMode = 1;
    if (_settingsButton && _controllerReader.dDown) _gameMode = 2;

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

void HybridController::processShieldManager()
{
    _shieldManager.setActivator(_rButton);
    _shieldManager.setLightShieldState(_cUpButton && _rButton);
    _shieldManager.process();

    r = _shieldManager.getHardShieldState();
    rAnalog = _shieldManager.getLightShieldOutput();
}

void HybridController::processLStick()
{
    _leftStick.setShieldState(_shieldManager);
    _leftStick.setXValue(_controllerReader.lsX);
    _leftStick.setYValue(_controllerReader.lsY);

    if (_trimLsXDown.justActivated()) trimLsXDown();
    if (_trimLsXUp.justActivated())   trimLsXUp();

    if (_trimLsYDown.justActivated()) trimLsYDown();
    if (_trimLsYUp.justActivated())   trimLsYUp();

    _leftStick.process();

    lsX = _leftStick.xValue;
    lsY = _leftStick.yValue;
}

void HybridController::processCStick()
{
    _cStick.setCLeftState(_cLeftButton);
    _cStick.setCRightState(_cRightButton);
    _cStick.setCDownState(_cDownButton);
    _cStick.setCUpState(_cUpButton);
    _cStick.setLsDownState(_lsDownButton);
    _cStick.setLsUpState(_lsUpButton);
    _cStick.setTiltState(_tiltButton);

    _cStick.process();

    cX = _cStick.xValue;
    cY = _cStick.yValue;

    // This allows cUp to be pressed while in shield to toggle light shield.
    if (_rButton && _cUpButton && !_jumpButton)
        cY = 128;
}

// Don't use pin 6 or 45 for buttons.
HybridController::HybridController()
: _controllerReader(17),
  // Buttons:
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
  _settingsButton(14)
{}

#endif // HYBRIDCONTROLLER_H

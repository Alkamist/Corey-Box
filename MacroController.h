#ifndef MACROCONTROLLER_H
#define MACROCONTROLLER_H

#include "ButtonReader.h"
#include "GameCubeController.h"
#include "ButtonOnlyCStick.h"
#include "ButtonOnlyLeftStick.h"
#include "WavedashMacro.h"

class MacroController : public GameCubeController
{
public:
    MacroController();

    void update();

private:
    ButtonReader _tiltButton;
    ButtonReader _shieldDropButton;

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
    ButtonReader _dLeftButton;
    ButtonReader _dRightButton;
    ButtonReader _dDownButton;
    ButtonReader _dUpButton;

    ButtonOnlyLeftStick _leftStick;
    ButtonOnlyCStick _cStick;

    WavedashMacro _wavedashMacro;

    void updateButtons();
    void setControls();
};



void MacroController::update()
{
    updateButtons();
    _cStick.update();
    _leftStick.update();
    _wavedashMacro.update();
    GameCubeController::update();

    setControls();
}

void MacroController::updateButtons()
{
    _tiltButton.update();
    _shieldDropButton.update();

    _lsLeftButton.update();
    _lsRightButton.update();
    _lsDownButton.update();
    _lsUpButton.update();

    _xMod1Button.update();
    _xMod2Button.update();
    _yMod1Button.update();
    _yMod2Button.update();

    _cLeftButton.update();
    _cRightButton.update();
    _cDownButton.update();
    _cUpButton.update();

    _aButton.update();
    _bButton.update();
    _jumpButton.update();
    _zButton.update();
    _rButton.update();
    _lButton.update();
    _startButton.update();
    _dLeftButton.update();
    _dRightButton.update();
    _dDownButton.update();
    _dUpButton.update();
}

void MacroController::setControls()
{
    bool allCButtons = _cLeftButton.isActive() && _cRightButton.isActive()
                    && _cDownButton.isActive() && _cUpButton.isActive();

    bool trimDown = allCButtons && _lsDownButton.isActive();
    bool trimUp = allCButtons && _lsUpButton.isActive();

    _wavedashMacro.setControls(_lButton, _rButton,
                               trimDown, trimUp);

    _leftStick.setControls(_lsLeftButton, _lsRightButton,
                           _lsDownButton, _lsUpButton,
                           _xMod1Button, _xMod2Button,
                           _yMod1Button, _yMod2Button,
                           _tiltButton, _wavedashMacro.getL(),
                           _shieldDropButton);

    _cStick.setControls(_cLeftButton, _cRightButton,
                        _cDownButton, _cUpButton,
                        _lsDownButton, _lsUpButton,
                        _tiltButton);

    a = _aButton;
    b = _bButton;
    //x = ?;
    y = _jumpButton.isActive() || _wavedashMacro.getJump();
    z = _zButton;
    l = _wavedashMacro.getL();
    r = _wavedashMacro.getR();
    start = _startButton;
    dLeft = _dLeftButton;
    dRight = _dRightButton;
    dDown = _dDownButton;
    dUp = _dUpButton;
    //lAnalog = ?;
    //rAnalog = ?
    lsX = _leftStick.getXControl();
    lsY = _leftStick.getYControl();
    cX = _cStick.getXControl();
    cY = _cStick.getYControl();
}

// Don't use pin 6 or 26 for buttons.
MacroController::MacroController()
: GameCubeController(),
  _tiltButton(27),
  _shieldDropButton(19),
  _lsLeftButton(0),
  _lsRightButton(3),
  _lsDownButton(1),
  _lsUpButton(2),
  _xMod1Button(8),
  _xMod2Button(5),
  _yMod1Button(7),
  _yMod2Button(4),
  _cLeftButton(9),
  _cRightButton(12),
  _cDownButton(10),
  _cUpButton(11),
  _aButton(14),
  _bButton(13),
  _jumpButton(16),
  _zButton(17),
  _lButton(15),
  _rButton(25),
  _startButton(24),
  _dLeftButton(21),
  _dRightButton(20),
  _dDownButton(22),
  _dUpButton(23),
  _leftStick(),
  _cStick()
{}

#endif // MACROCONTROLLER_H
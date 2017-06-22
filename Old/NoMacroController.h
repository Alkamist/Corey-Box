#ifndef NOMACROCONTROLLER_H
#define NOMACROCONTROLLER_H

#include "ButtonReader.h"
#include "GameCubeControls.h"
#include "ButtonOnlyCStick.h"
#include "ButtonOnlyLeftStick.h"

class NoMacroController
{
public:
    NoMacroController();

    void update();

    const GameCubeControls& getControls() const { return _controls; }

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
    ButtonReader _rButton;
    ButtonReader _lButton;
    ButtonReader _startButton;
    ButtonReader _dLeftButton;
    ButtonReader _dRightButton;
    ButtonReader _dDownButton;
    ButtonReader _dUpButton;

    ButtonOnlyLeftStick _leftStick;
    ButtonOnlyCStick _cStick;

    GameCubeControls _controls;

    void updateButtons();
    void applyControls();
};



void NoMacroController::update()
{
    updateButtons();

    _cStick.update();
    _leftStick.update();
    _controls.update();

    applyControls();
}

void NoMacroController::updateButtons()
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
    //_xButton.update();
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

void NoMacroController::applyControls()
{
    _controls.a.setValue(_aButton.isActive());
    _controls.b.setValue(_bButton.isActive());
    //_controls.x.setValue(_xButton);
    _controls.y.setValue(_jumpButton.isActive());
    _controls.z.setValue(_zButton.isActive());
    _controls.l.setValue(_lButton.isActive());
    _controls.r.setValue(_rButton.isActive());
    _controls.start.setValue(_startButton.isActive());
    _controls.dLeft.setValue(_dLeftButton.isActive());
    _controls.dRight.setValue(_dRightButton.isActive());
    _controls.dDown.setValue(_dDownButton.isActive());
    _controls.dUp.setValue(_dUpButton.isActive());
    //_controls.lAnalog
    //_controls.rAnalog
    _controls.lsX.setValue(_leftStick.getXValue());
    _controls.lsY.setValue(_leftStick.getYValue());
    _controls.cX.setValue(_cStick.getXValue());
    _controls.cY.setValue(_cStick.getYValue());
}

// Don't use pin 6 or 26
NoMacroController::NoMacroController()
: _tiltButton(27),
  _lsLeftButton(0),
  _lsDownButton(1),
  _lsUpButton(2),
  _lsRightButton(3),
  _yMod2Button(4),
  _xMod2Button(5),
  _yMod1Button(7),
  _xMod1Button(8),
  _cLeftButton(9),
  _cDownButton(10),
  _cUpButton(11),
  _cRightButton(12),
  _bButton(13),
  _aButton(14),
  _lButton(15),
  _jumpButton(16),
  _zButton(17),
  _rButton(25),
  _startButton(24),
  _dUpButton(23),
  _dDownButton(22),
  _dLeftButton(21),
  _dRightButton(20),
  _shieldDropButton(19),
  _leftStick(_lsLeftButton, _lsRightButton,
             _xMod1Button, _xMod2Button,
             _lsDownButton, _lsUpButton,
             _yMod1Button, _yMod2Button,
             _tiltButton, _lButton,
             _shieldDropButton),
  _cStick(_cLeftButton, _cRightButton,
          _cDownButton, _cUpButton,
          _lsDownButton, _lsUpButton,
          _tiltButton)
{}

#endif // NOMACROCONTROLLER_H
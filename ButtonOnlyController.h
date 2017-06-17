#ifndef BUTTONONLYCONTROLLER_H
#define BUTTONONLYCONTROLLER_H

#include "ButtonReader.h"
#include "GameCubeControls.h"
#include "ButtonOnlyCStick.h"
#include "ButtonOnlyLeftStick.h"

class ButtonOnlyController
{
public:
    ButtonOnlyController();

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
    //ButtonReader _xButton;
    ButtonReader _yButton;
    ButtonReader _zButton;
    ButtonReader _rButton;
    ButtonReader _lButton;
    ButtonReader _startButton;
    //ButtonReader _dLeftButton;
    //ButtonReader _dRightButton;
    //ButtonReader _dDownButton;
    //ButtonReader _dUpButton;

    ButtonOnlyLeftStick _leftStick;
    ButtonOnlyCStick _cStick;

    GameCubeControls _controls;

    void updateButtons();
    void applyControls();
};



void ButtonOnlyController::update()
{
    updateButtons();
    _cStick.update();
    _leftStick.update();
    _controls.update();

    applyControls();
}

void ButtonOnlyController::updateButtons()
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
    _yButton.update();
    _zButton.update();
    _rButton.update();
    _lButton.update();
    _startButton.update();
    //_dLeftButton.update();
    //_dRightButton.update();
    //_dDownButton.update();
    //_dUpButton.update();
}

void ButtonOnlyController::applyControls()
{
    _controls.a.setValue(_aButton.isActive());
    _controls.b.setValue(_bButton.isActive());
    //_controls.x.setValue(_xButton);
    _controls.y.setValue(_yButton.isActive());
    _controls.z.setValue(_zButton.isActive());
    _controls.l.setValue(_lButton.isActive());
    _controls.r.setValue(_rButton.isActive());
    _controls.start.setValue(_startButton.isActive());
    //_controls.dLeft.setValue(_dLeftButton.isActive());
    //_controls.dRight.setValue(_dRightButton.isActive());
    //_controls.dDown.setValue(_dDownButton.isActive());
    //_controls.dUp.setValue(_dUpButton.isActive());
    //_controls.lAnalogButton);
    //_controls.rAnalogButton);
    _controls.lsX.setValue(_leftStick.getXValue());
    _controls.lsY.setValue(_leftStick.getYValue());
    _controls.cX.setValue(_cStick.getXValue());
    _controls.cY.setValue(_cStick.getYValue());
}

// Don't use pin 6 or 26
ButtonOnlyController::ButtonOnlyController()
: _tiltButton(27),
  _shieldDropButton(0),
  _lsLeftButton(1),
  _lsRightButton(4),
  _lsDownButton(2),
  _lsUpButton(3),
  _xMod1Button(9),
  _xMod2Button(7),
  _yMod1Button(8),
  _yMod2Button(5),
  _cLeftButton(10),
  _cRightButton(13),
  _cDownButton(11),
  _cUpButton(12),
  _aButton(15),
  _bButton(14),
  _yButton(17),
  _zButton(18),
  _rButton(19),
  _lButton(16),
  _startButton(19),
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

#endif // BUTTONONLYCONTROLLER_H
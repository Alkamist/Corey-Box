#ifndef BUTTONONLYCONTROLLER_H
#define BUTTONONLYCONTROLLER_H

#include "ButtonReader.h"
#include "TwoButtonControl.h"
#include "ModdedAxis.h"
#include "AxisModifier.h"
#include "GameCubeControls.h"

class ButtonOnlyController
{
public:
    ButtonOnlyController();

    void update();

    GameCubeControls getControls() { return _controls; }

private:
    ButtonReader _tiltButton;
    //ButtonReader _shieldDropButton;

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

    //CombinedModAxis _lsX;
    //CombinedModAxis _lsY;
    //TwoButtonControl _cX;
    //CStickYAxis _cY;

    GameCubeControls _controls;

    void updateButtons();
    void updateAxes();
    void applyControls();
};



void ButtonOnlyController::update()
{
    updateButtons();
    //_lsX.update();
    //_lsY.update();
    //_cX.update();
    //_cY.update();
    _controls.update();

    applyControls();
}

void ButtonOnlyController::updateButtons()
{
    _tiltButton.update();
    //_shieldDropButton.update();

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
    _controls.a = _aButton;
    _controls.b = _bButton;
    //_controls.x = _xButton;
    _controls.y = _yButton;
    _controls.z = _zButton;
    _controls.l = _lButton;
    _controls.r = _rButton;
    _controls.start = _startButton;
    //_controls.dLeft = _dLeftButton;
    //_controls.dRight = _dRightButton;
    //_controls.dDown = _dDownButton;
    //_controls.dUp = _dUpButton;
    //_controls.lAnalogButton;
    //_controls.rAnalogButton;
    _controls.lsX = _lsX;
    _controls.lsY = _lsY;
    _controls.cX = _cX;
    _controls.cY = _cY;
}

// Don't use pin 6 or 26
ButtonOnlyController::ButtonOnlyController()
: _tiltButton(27),
  _lsLeftButton(0),
  _lsRightButton(3),
  _lsDownButton(1),
  _lsUpButton(2),
  _xMod1Button(8),
  _xMod2Button(5),
  _yMod1Button(7),
  _yMod2Button(4),
  _cLeftButton(9),
  _cRightButton(11),
  _cDownButton(10),
  _cUpButton(12),
  _aButton(14),
  _bButton(13),
  _yButton(17),
  _zButton(18),
  _rButton(19),
  _lButton(15),
  _startButton(20)
{}

#endif // BUTTONONLYCONTROLLER_H
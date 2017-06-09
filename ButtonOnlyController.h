#ifndef BUTTONONLYCONTROLLER_H
#define BUTTONONLYCONTROLLER_H

#include "ButtonReader.h"
#include "TwoButtonControl.h"
#include "AxisModifier.h"
#include "GameCubeControls.h"

class ButtonOnlyController
{
public:
    ButtonOnlyController();

    GameCubeControls getControls() { return _controls; }

    void update();

private:
    ButtonReader _tilt;
    ButtonReader _lsLeft;
    ButtonReader _lsRight;
    ButtonReader _lsDown;
    ButtonReader _lsUp;

    ButtonReader _xMod1;
    ButtonReader _xMod2;
    ButtonReader _yMod1;
    ButtonReader _yMod2;

    ButtonReader _cLeft;
    ButtonReader _cRight;
    ButtonReader _cDown;
    ButtonReader _cUp;

    ButtonReader _a;
    ButtonReader _b;
    ButtonReader _x;
    ButtonReader _y;
    ButtonReader _z;
    ButtonReader _r;
    ButtonReader _l;
    ButtonReader _start;
    ButtonReader _dLeft;
    ButtonReader _dRight;
    ButtonReader _dDown;
    ButtonReader _dUp;

    TwoButtonControl _lsX;
    TwoButtonControl _lsY;
    TwoButtonControl _cX;
    TwoButtonControl _cY;

    AxisModifier _mod1;
    AxisModifier _mod2;
    AxisModifier _mod3;
    AxisModifier _tiltMod;

    GameCubeControls _controls;

    void updateAnalog();
    void updateButtons();
    void applyControls();
    void bindTwoButtonControls();
    void applyModifiers();
};



void ButtonOnlyController::update()
{
    updateButtons();
    updateAnalog();
    _controls.update();

    bindTwoButtonControls();
    applyModifiers();

    applyControls();
}

void ButtonOnlyController::applyControls()
{
    _controls.a = _a;
    _controls.b = _b;
    _controls.x = _x;
    _controls.y = _y;
    _controls.z = _z;
    _controls.l = _l;
    _controls.r = _r;
    _controls.start = _start;
    _controls.dLeft = _dLeft;
    _controls.dRight = _dRight;
    _controls.dDown = _dDown;
    _controls.dUp = _dUp;
    //_controls.lAnalog;
    //_controls.rAnalog;
    _controls.lsX = _lsX;
    _controls.lsY = _lsY;
    _controls.cX = _cX;
    _controls.cY = _cY;
}

void ButtonOnlyController::bindTwoButtonControls()
{
    _lsX.setLowControl(_lsLeft);
    _lsX.setHighControl(_lsRight);

    _lsY.setLowControl(_lsDown);
    _lsY.setHighControl(_lsUp);

    _cX.setLowControl(_cLeft);
    _cX.setHighControl(_cRight);

    _cY.setLowControl(_cDown);
    _cY.setHighControl(_cUp);
}

void ButtonOnlyController::applyModifiers()
{

}

void ButtonOnlyController::updateAnalog()
{
    _lsX.update();
    _lsY.update();
    _cX.update();
    _cY.update();

    _mod1.update();
    _mod2.update();
    _mod3.update();
    _tiltMod.update();
}

void ButtonOnlyController::updateButtons()
{
    _tilt.update();
    _lsLeft.update();
    _lsRight.update();
    _lsDown.update();
    _lsUp.update();

    _xMod1.update();
    _xMod2.update();
    _yMod1.update();
    _yMod2.update();

    _cLeft.update();
    _cRight.update();
    _cDown.update();
    _cUp.update();

    _a.update();
    _b.update();
    _x.update();
    _y.update();
    _z.update();
    _r.update();
    _l.update();
    _start.update();
    _dLeft.update();
    _dRight.update();
    _dDown.update();
    _dUp.update();
}

// Don't use pin 6 or 26
ButtonOnlyController::ButtonOnlyController()
: _tilt(27),
  _lsLeft(0),
  _lsRight(3),
  _lsDown(1),
  _lsUp(2),
  _xMod1(8),
  _xMod2(5),
  _yMod1(7),
  _yMod2(4),
  _cLeft(9),
  _cRight(11),
  _cDown(10),
  _cUp(12),
  _a(14),
  _b(13),
  _x(16),
  _y(17),
  _z(18),
  _r(19),
  _l(15),
  _start(20),
  _dLeft(21),
  _dRight(24),
  _dDown(22),
  _dUp(23)
{
    _mod1.setValue(0.25);
    _mod2.setValue(0.50);
    _mod3.setValue(0.75);
    _tiltMod.setValue(0.40);
}

#endif // BUTTONONLYCONTROLLER_H
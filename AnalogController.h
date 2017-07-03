#ifndef ANALOGCONTROLLER_H
#define ANALOGCONTROLLER_H

#include "GameCubeController.h"
#include "ButtonReader.h"
#include "ButtonOnlyCStick.h"
#include "AnalogLeftStick.h"
#include "WavedashMacro.h"
#include "TwoButtonSpamMacro.h"
#include "TwoButtonStateTracker.h"
#include "ToggleActivator.h"
#include "GameCubeControllerReader.h"

class AnalogController : public GameCubeController
{
public:
    AnalogController();

    void process();
    void endCycle();

private:
    GameCubeControllerReader _inputController;

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

    ButtonOnlyCStick _cStick;
    AnalogLeftStick _leftStick;

    WavedashMacro _wavedashMacro;
    TwoButtonStateTracker _spamBAActivator;
    TwoButtonSpamMacro _spamBAMacro;

    Activator _trimWavedashDown;
    Activator _trimWavedashUp;
    Activator _trimShieldDropDown;
    Activator _trimShieldDropUp;

    Activator _disableMacros;
    ToggleActivator _macrosAreOn;
};



void AnalogController::process()
{
    _inputController.process();

    // ====================== OUTPUT INITIALIZATION ======================
    a = _aButton;
    b = _bButton;
    x = false;
    y = _jumpButton;
    z = _zButton;
    l = _lButton;
    r = _rButton;
    //start = _inputController.start;
    //dLeft = _inputController.dLeft;
    //dRight = _inputController.dRight;
    //dDown = _inputController.dDown;
    //dUp = _inputController.dUp;
    //lAnalog = ;
    //rAnalog = ;
    //lsX = _inputController.lsX;
    //lsY = _inputController.lsY;
    //cX = ;
    //cY = ;

    //_leftStick.setWavedashState(false);


    // ====================== BUTTON COMBOS ======================
    bool extraButtonCombo = dDown;
    _disableMacros = dLeft && _cUpButton;
    _macrosAreOn.setActivatorState(_disableMacros.justActivated());
    _macrosAreOn.process();

    _trimWavedashDown = dRight && _cDownButton;
    _trimWavedashUp = dRight && _cUpButton;
    _trimShieldDropDown = dDown && _cDownButton;
    _trimShieldDropUp = dDown && _cUpButton;



    // ====================== SPAM B OR BA MACRO ======================
    _spamBAActivator.setState1(_bButton);
    _spamBAActivator.setState2(_aButton);
    _spamBAActivator.process();

    bool spamBACombo = _spamBAActivator.state1WasFirst() && _spamBAActivator.getState2();

    _spamBAMacro.setActivatorState(spamBACombo && _macrosAreOn);
    _spamBAMacro.process();

    if (_spamBAMacro.isRunning())
    {
        b = _spamBAMacro.getButton1();

        if (spamBACombo && _cDownButton) a = _spamBAMacro.getButton2();
    }



    // ====================== WAVEDASH MACRO ======================
    _wavedashMacro.setActivatorState(_lButton && !extraButtonCombo && _macrosAreOn);

    if (_trimWavedashDown.justActivated()) _wavedashMacro.trimDown();
    if (_trimWavedashUp.justActivated())   _wavedashMacro.trimUp();

    _wavedashMacro.process();

    if (extraButtonCombo)
    {
        y = false;
        x = _jumpButton;
    }

    if (_macrosAreOn && !extraButtonCombo)
    {
        y = _jumpButton;
        if (_wavedashMacro.getJump().isRunning()) y = _wavedashMacro.getJump();

        r = _rButton;
        if (_wavedashMacro.getR().isRunning()) r = _wavedashMacro.getR();

        l = _wavedashMacro.getL();
    }



    // ====================== LEFT STICK ======================
    _leftStick.setBackdashFixOverride(a || b || x || y || z || l || r);
    //_leftStick.setXValue(_inputController.lsX);
    //_leftStick.setYValue(_inputController.lsY);

    _leftStick.process();

    lsX = _leftStick.getXValue();
    lsY = _leftStick.getYValue();



    // ====================== C STICK ======================
    _cStick.setCLeftState(_cLeftButton);
    _cStick.setCRightState(_cRightButton);
    _cStick.setCDownState(_cDownButton && !spamBACombo);
    _cStick.setCUpState(_cUpButton);

    _cStick.process();

    cX = _cStick.getXValue();
    cY = _cStick.getYValue();
}

void AnalogController::endCycle()
{
    GameCubeController::endCycle();
    _inputController.endCycle();

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

    // Outputs:
    _trimShieldDropDown.endCycle();
    _trimShieldDropUp.endCycle();
    _trimWavedashDown.endCycle();
    _trimWavedashUp.endCycle();

    // Sticks:
    _cStick.endCycle();
    _leftStick.endCycle();

    // Macros:
    _wavedashMacro.endCycle();
    _spamBAMacro.endCycle();

    _disableMacros.endCycle();
    _macrosAreOn.endCycle();
}

// Don't use pin 6 or 26 for buttons.
AnalogController::AnalogController()
: _inputController(17),
  _cLeftButton(8),
  _cRightButton(4),
  _cDownButton(9),
  _cUpButton(3),
  _aButton(7),
  _bButton(5),
  _jumpButton(1),
  _zButton(0),
  _lButton(2),
  _rButton(27),
  _macrosAreOn(true)
{}

#endif // ANALOGCONTROLLER_H

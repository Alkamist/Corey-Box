#ifndef BUTTONONLYCONTROLLER_H
#define BUTTONONLYCONTROLLER_H

#include "ButtonReader.h"
#include "GameCubeController.h"
#include "ButtonOnlyCStick.h"
#include "ButtonOnlyLeftStick.h"
#include "WavedashMacro.h"
#include "TwoButtonSpamMacro.h"
#include "TwoButtonStateTracker.h"
#include "ToggleActivator.h"

// This is the main controller class right now. I know it is kind of a
// god class but I'm not sure how to split up this logic.
class ButtonOnlyController : public GameCubeController
{
public:
    ButtonOnlyController();

    void process();
    void endCycle();

    const ButtonReader& getLButton() const { return _lButton; }

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
    TwoButtonStateTracker _spamBAActivator;
    TwoButtonSpamMacro _spamBAMacro;

    Activator _trimWavedashDown;
    Activator _trimWavedashUp;
    Activator _trimShieldDropDown;
    Activator _trimShieldDropUp;
    Activator _trimModsInward;
    Activator _trimModsOutward;

    Activator _meleeMode;
    Activator _projectMMode;

    Activator _disableMacros;
    ToggleActivator _macrosAreOn;
};



void ButtonOnlyController::process()
{
    // ====================== OUTPUT INITIALIZATION ======================
    a = _aButton;
    b = _bButton;
    x = false;
    y = _jumpButton;
    z = _zButton;
    l = _lButton;
    r = _rButton;
    start = _startButton;
    dLeft = _dLeftButton;
    dRight = _dRightButton;
    dDown = _dDownButton;
    dUp = _dUpButton;
    _leftStick.setWavedashState(false);



    // ====================== BUTTON COMBOS ======================
    bool extraButtonCombo = _yMod1Button && _xMod1Button;
    bool wavedashCombo = _xMod1Button && _xMod2Button && _yMod1Button && _yMod2Button;
    bool analogCombo = extraButtonCombo && _shieldDropButton;
    _disableMacros = _xMod1Button && _xMod2Button && _yMod1Button && _yMod2Button && _dUpButton && _shieldDropButton;
    _macrosAreOn.setActivatorState(_disableMacros.justActivated());
    _macrosAreOn.process();

    _trimWavedashDown = wavedashCombo && _cDownButton;
    _trimWavedashUp = wavedashCombo && _cUpButton;
    _trimShieldDropDown = analogCombo && _cDownButton;
    _trimShieldDropUp = analogCombo && _cUpButton;
    _trimModsInward = analogCombo && _cLeftButton;
    _trimModsOutward = analogCombo && _cRightButton;

    _meleeMode = analogCombo && _aButton;
    _projectMMode = analogCombo && _bButton;



    // ====================== SPAM B OR BA MACRO ======================
    _spamBAActivator.setState1(_bButton);
    _spamBAActivator.setState2(_aButton);
    _spamBAActivator.process();

    bool spamABKey = _spamBAActivator.state1WasFirst() && _spamBAActivator.getState2();

    _spamBAMacro.setActivatorState(spamABKey && _macrosAreOn);
    _spamBAMacro.process();

    if (_spamBAMacro.isRunning())
    {
        b = _spamBAMacro.getButton1();

        if (spamABKey && _cDownButton) a = _spamBAMacro.getButton2();
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
    _leftStick.setShieldDropState(_shieldDropButton);

    if (_meleeMode.justActivated())
    {
        _leftStick.resetMods();
        _leftStick.setShieldDrop(-0.67500);
    }
    if (_projectMMode.justActivated())
    {
        _leftStick.setModStart(0.42);
        _leftStick.setShieldDrop(-0.90);
    }

    if (_trimShieldDropDown.justActivated()) _leftStick.trimShieldDropDown();
    if (_trimShieldDropUp.justActivated())   _leftStick.trimShieldDropUp();
    if (_trimModsInward.justActivated())     _leftStick.trimModsInward();
    if (_trimModsOutward.justActivated())    _leftStick.trimModsOutward();

    _leftStick.process();

    lsX = _leftStick.getXControl().getValue();
    lsY = _leftStick.getYControl().getValue();



    // ====================== C STICK ======================
    _cStick.setCLeftState(_cLeftButton);
    _cStick.setCRightState(_cRightButton);
    _cStick.setCDownState(_cDownButton && !spamABKey);
    _cStick.setCUpState(_cUpButton);
    _cStick.setLsDownState(_lsDownButton);
    _cStick.setLsUpState(_lsUpButton);
    _cStick.setTiltState(_tiltButton);

    _cStick.process();

    cX = _cStick.getXControl().getValue();
    cY = _cStick.getYControl().getValue();
}

void ButtonOnlyController::endCycle()
{
    GameCubeController::endCycle();

    // Buttons:
    _tiltButton.endCycle();
    _shieldDropButton.endCycle();

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
    _dLeftButton.endCycle();
    _dRightButton.endCycle();
    _dDownButton.endCycle();
    _dUpButton.endCycle();

    _trimShieldDropDown.endCycle();
    _trimShieldDropUp.endCycle();
    _trimWavedashDown.endCycle();
    _trimWavedashUp.endCycle();
    _trimModsInward.endCycle();
    _trimModsOutward.endCycle();

    _meleeMode.endCycle();
    _projectMMode.endCycle();

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
ButtonOnlyController::ButtonOnlyController()
: // Buttons:
  _tiltButton(27),
  _shieldDropButton(9),
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
  _dLeftButton(12),
  _dRightButton(14),
  _dDownButton(13),
  _dUpButton(10),
  _macrosAreOn(true)
{}

#endif // BUTTONONLYCONTROLLER_H

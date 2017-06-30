#ifndef MACROCONTROLLER_H
#define MACROCONTROLLER_H

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
class MacroController : public GameCubeController
{
public:
    MacroController();

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

    Activator _aOut;
    Activator _bOut;
    Activator _xOut;
    Activator _yOut;
    Activator _lOut;
    Activator _rOut;
    UnipolarControl _rAnalogOut;
    BipolarControl _lsXOut;
    BipolarControl _lsYOut;
    BipolarControl _cXOut;
    BipolarControl _cYOut;

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



void MacroController::process()
{
    // ====================== OUTPUT INITIALIZATION ======================
    _aOut = _aButton;
    _bOut = _bButton;
    _xOut = false;
    _yOut = _jumpButton;
    _lOut = _lButton;
    _rOut = _rButton;
    _leftStick.setWavedashState(false);


    // ====================== BUTTON COMBOS ======================
    bool extraButtonKey = _yMod2Button && _xMod1Button;
    bool wavedashKey = _xMod1Button && _xMod2Button && _yMod1Button && _yMod2Button;
    bool analogKey = _yMod2Button && _xMod1Button && _shieldDropButton;
    _disableMacros = _xMod1Button && _xMod2Button && _yMod1Button && _yMod2Button && _dUpButton && _shieldDropButton;
    _macrosAreOn.setActivatorState(_disableMacros.justActivated());
    _macrosAreOn.process();

    _trimWavedashDown = wavedashKey && _cDownButton;
    _trimWavedashUp = wavedashKey && _cUpButton;
    _trimShieldDropDown = analogKey && _cDownButton;
    _trimShieldDropUp = analogKey && _cUpButton;
    _trimModsInward = analogKey && _cLeftButton;
    _trimModsOutward = analogKey && _cRightButton;

    _meleeMode = analogKey && _aButton;
    _projectMMode = analogKey && _bButton;



    // ====================== SPAM B OR BA MACRO ======================
    _spamBAActivator.setState1(_bButton);
    _spamBAActivator.setState2(_aButton);
    _spamBAActivator.process();

    bool spamABKey = _spamBAActivator.state1WasFirst() && _spamBAActivator.getState2();

    _spamBAMacro.setActivatorState(spamABKey && _macrosAreOn);
    _spamBAMacro.process();

    if (_spamBAMacro.isRunning())
    {
        _bOut = _spamBAMacro.getButton1();

        if (spamABKey && _cDownButton) _aOut = _spamBAMacro.getButton2();
    }



    // ====================== WAVEDASH MACRO ======================
    _wavedashMacro.setActivatorState(_lButton && !extraButtonKey && _macrosAreOn);

    if (_trimWavedashDown.justActivated()) _wavedashMacro.trimDown();
    if (_trimWavedashUp.justActivated())   _wavedashMacro.trimUp();

    _wavedashMacro.process();

    if (extraButtonKey)
    {
        _yOut = false;
        _xOut = _jumpButton;
    }

    if (_macrosAreOn && !extraButtonKey)
    {
        _yOut = _jumpButton;
        if (_wavedashMacro.getJump().isRunning()) _yOut = _wavedashMacro.getJump();

        _rOut = _rButton;
        if (_wavedashMacro.getR().isRunning()) _rOut = _wavedashMacro.getR();

        _lOut = _wavedashMacro.getL();
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
    _leftStick.setTiltTempDisableState(_lOut);
    _leftStick.setWavedashState(_wavedashMacro.getL());
    _leftStick.setShieldDropState(_shieldDropButton);

    if (_meleeMode.justActivated())
    {
        _leftStick.setRange(0.6250);
        _leftStick.resetMods();
        _leftStick.resetShieldDrop();
        _cStick.setRange(0.6250);
    }
    if (_projectMMode.justActivated())
    {
        _leftStick.setRange(0.8000);
        _leftStick.resetMods();
        _leftStick.resetShieldDrop();
        _cStick.setRange(0.8000);
    }

    if (_trimShieldDropDown.justActivated()) _leftStick.trimShieldDropDown();
    if (_trimShieldDropUp.justActivated())   _leftStick.trimShieldDropUp();
    if (_trimModsInward.justActivated())     _leftStick.trimModsInward();
    if (_trimModsOutward.justActivated())    _leftStick.trimModsOutward();

    _leftStick.process();

    _lsXOut.setValue(_leftStick.getXControl().getValue());
    _lsYOut.setValue(_leftStick.getYControl().getValue());



    // ====================== C STICK ======================
    _cStick.setCLeftState(_cLeftButton);
    _cStick.setCRightState(_cRightButton);
    _cStick.setCDownState(_cDownButton && !spamABKey);
    _cStick.setCUpState(_cUpButton);
    _cStick.setLsDownState(_lsDownButton);
    _cStick.setLsUpState(_lsUpButton);
    _cStick.setTiltState(_tiltButton);

    _cStick.process();

    _cXOut.setValue(_cStick.getXControl().getValue());
    _cYOut.setValue(_cStick.getYControl().getValue());
}

void MacroController::endCycle()
{
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

    // Outputs:
    _xOut.endCycle();
    _yOut.endCycle();
    _lOut.endCycle();
    _rOut.endCycle();
    _bOut.endCycle();
    _aOut.endCycle();
    _rAnalogOut.endCycle();
    _lsXOut.endCycle();
    _lsYOut.endCycle();
    _cXOut.endCycle();
    _cYOut.endCycle();

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
MacroController::MacroController()
: // Buttons:
  _tiltButton(27),
  _shieldDropButton(8),
  _lsLeftButton(0),
  _lsRightButton(3),
  _lsDownButton(1),
  _lsUpButton(2),
  _xMod1Button(19),
  _xMod2Button(23),
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
  _dUpButton(5),
  _macrosAreOn(true)
{
    a = &_aOut;
    b = &_bOut;
    x = &_xOut;
    y = &_yOut;
    z = &_zButton;
    l = &_lOut;
    r = &_rOut;
    start = &_startButton;
    dLeft = &_dLeftButton;
    dRight = &_dRightButton;
    dDown = &_dDownButton;
    dUp = &_dUpButton;
    //lAnalog = &_lAnalogOut;
    rAnalog = &_rAnalogOut;
    lsX = &_lsXOut;
    lsY = &_lsYOut;
    cX = &_cXOut;
    cY = &_cYOut;
}

#endif // MACROCONTROLLER_H

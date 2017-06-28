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

    Activator _xOut;
    Activator _yOut;
    Activator _lOut;
    Activator _rOut;
    Activator _wavedashOut;
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
};



void MacroController::process()
{
    bool wavedashKey = _xMod1Button && _xMod2Button && _yMod1Button && _yMod2Button;
    bool analogKey = _yMod2Button && _xMod1Button && _shieldDropButton;

    // Wavedash Macro:
    _trimWavedashDown.setState(wavedashKey && _cDownButton);
    _trimWavedashUp.setState(wavedashKey && _cUpButton);

    _wavedashMacro.process();

    _yOut.setState(_jumpButton);
    if (_wavedashMacro.getJump().isRunning())
        _yOut.setState(_wavedashMacro.getJump());

    _rOut.setState(_rButton);
    if (_wavedashMacro.getR().isRunning())
        _rOut.setState(_wavedashMacro.getR());

    _lOut.setState(_wavedashMacro.getL());
    _wavedashOut.setState(_wavedashMacro.getL());

    // Sticks:
    _leftStick.process();
    _cStick.process();

    _lsXOut.setValue(_leftStick.getXControl().getValue());
    _lsYOut.setValue(_leftStick.getYControl().getValue());
    _cXOut.setValue(_cStick.getXControl().getValue());
    _cYOut.setValue(_cStick.getYControl().getValue());

    // Shield Drop Trim:
    _trimShieldDropDown.setState(analogKey && _cDownButton);
    _trimShieldDropUp.setState(analogKey && _cUpButton);

    if (_trimShieldDropDown.justActivated())
        _leftStick.trimShieldDropDown();

    if (_trimShieldDropUp.justActivated())
        _leftStick.trimShieldDropUp();

    // Angle Trim:
    _trimModsInward.setState(analogKey && _cLeftButton);
    _trimModsOutward.setState(analogKey && _cRightButton);

    if (_trimModsInward.justActivated())
        _leftStick.trimModsInward();

    if (_trimModsOutward.justActivated())
        _leftStick.trimModsOutward();

    // Controller Mode:
    _meleeMode.setState(analogKey && _aButton);
    _projectMMode.setState(analogKey && _bButton);

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
    _wavedashOut.endCycle();
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
}

// Don't use pin 6 or 26 for buttons.
MacroController::MacroController()
: // Buttons:
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

  // Sticks:
  _leftStick(_lsLeftButton, _lsRightButton,
             _lsDownButton, _lsUpButton,
             _xMod1Button, _xMod2Button,
             _yMod1Button, _yMod2Button,
             _tiltButton, _wavedashOut,
             _shieldDropButton),

  _cStick(_cLeftButton, _cRightButton,
          _cDownButton, _cUpButton,
          _lsDownButton, _lsUpButton,
          _tiltButton),

  // Macros:
  _wavedashMacro(_lButton, _trimWavedashDown, _trimWavedashUp)
{
    a = &_aButton;
    b = &_bButton;
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

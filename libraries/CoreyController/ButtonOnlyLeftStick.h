#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "FightingJoystick.h"
#include "JoystickTilter.h"
#include "JoystickShieldDropper.h"
#include "JoystickBackdashFixer.h"
#include "DoubleModAxis.h"
#include "Signum.h"

// This class represents the left analog stick used in the button only controller.
class ButtonOnlyLeftStick : public FightingJoystick
{
public:
    ButtonOnlyLeftStick()
    : FightingJoystick(),
      _lsLeftState(false),
      _lsRightState(false),
      _lsDownState(false),
      _lsUpState(false),
      _tiltState(false),
      _tiltOut(false)
    {
        _tiltTempDisable.setTime(5);
        _wavedashState.setTime(5);
    }

    void process()
    {
        _wavedashState.process();
        _tiltTempDisable.process();
        _tiltOut = _tiltState && !(_tiltTempDisable || _wavedashState);

        _xAxis.process();
        _yAxis.process();

        xValue = _xAxis;
        yValue = _yAxis;

        Joystick::process();

        _joystickBackdashFixer.process(*this);

        if (_wavedashState && (yValue > (128 - getTiltLowerBound())))
            yValue = 128 - _yAxis.getMod1Value();

        Joystick::process();

        _joystickTilter.setTiltState(_tiltOut);
        _joystickTilter.process(*this);

        _joystickShieldDropper.process(*this);
    }

    void endCycle()
    {
        Joystick::endCycle();
        _joystickTilter.endCycle();
        _joystickBackdashFixer.endCycle();
        _tiltTempDisable.endCycle();
        _wavedashState.endCycle();
        _xAxis.endCycle();
        _yAxis.endCycle();
    }

    void setLsLeftState(const bool state)             { _lsLeftState = state; _xAxis.setLowState(state); }
    void setLsRightState(const bool state)            { _lsRightState = state; _xAxis.setHighState(state); }
    void setLsDownState(const bool state)             { _lsDownState = state; _yAxis.setLowState(state); }
    void setLsUpState(const bool state)               { _lsUpState = state; _yAxis.setHighState(state); }
    void setXMod1State(const bool state)              { _xAxis.setMod1State(state); }
    void setXMod2State(const bool state)              { _xAxis.setMod2State(state); }
    void setYMod1State(const bool state)              { _yAxis.setMod1State(state); }
    void setYMod2State(const bool state)              { _yAxis.setMod2State(state); }
    void setTiltState(const bool state)               { _tiltState = state; }
    void setTiltTempDisableState(const bool state)    { _tiltTempDisable = state; }
    void setWavedashState(const bool state)           { _wavedashState = state; }
    void setShieldDropState(const bool state)         { _joystickShieldDropper.setShieldDropState(state); }
    void setShieldState(const bool state)             { _joystickShieldDropper.setShieldState(state); }
    void setBackdashFixDisableState(const bool state) { _joystickBackdashFixer.setBackdashFixDisableState(state); }

    void setShieldDrop(const uint8_t value)           { _joystickShieldDropper.setShieldDrop(value); }
    void setModStart(const uint8_t value)             { _xAxis.setModStart(value); _yAxis.setModStart(value); }
    void setTilt(const uint8_t value)                 { _joystickTilter.setTilt(value); }

    void setRange(const uint8_t value)
    {
        FightingJoystick::setRange(value);
        _xAxis.setRange(value);
        _yAxis.setRange(value);
        _joystickTilter.setRange(value);
    }

private:
    JoystickTilter _joystickTilter;
    JoystickShieldDropper _joystickShieldDropper;
    JoystickBackdashFixer _joystickBackdashFixer;

    bool _lsLeftState;
    bool _lsRightState;
    bool _lsDownState;
    bool _lsUpState;
    bool _tiltState;
    bool _tiltOut;
    TemporaryActivator _tiltTempDisable;
    TemporaryActivator _wavedashState;

    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;
};

#endif // BUTTONONLYLEFTSTICK_H

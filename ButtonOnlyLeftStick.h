#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "TiltJoystick.h"
#include "DoubleModAxis.h"
#include "Signum.h"

// This class represents the left analog stick used in the button only controller.
class ButtonOnlyLeftStick : public TiltJoystick
{
public:
    ButtonOnlyLeftStick()
    : TiltJoystick(),
      _tiltState(false),
      _tiltOut(false),
      _shieldDropValue(-0.67500)
    {
        _tiltTempDisable.setTime(frames(5.0));
        _wavedashState.setTime(frames(5.0));
        _shieldDropNeutral.setTime(frames(1.0));
    }

    void process()
    {
        _shieldDropNeutral.setActivatorState(_shieldDropState.justActivated());
        _shieldDropNeutral.process();

        _tiltTempDisable.process();
        _wavedashState.process();

        _xAxis.process();
        _yAxis.process();

        if (_wavedashState && (_yAxis.getValue() > -0.33250))
        {
            _yAxis.setValue(-0.33250);
        }

        if (_shieldDropState)
        {
            if (_shieldDropNeutral)
            {
                _xAxis.setValue(0.0);
                _yAxis.setValue(0.0);
            }
            else
            {
                _xAxis.setValue(0.0);
                _yAxis.setValue(_shieldDropValue);
            }
        }

        _tiltOut = _tiltState && !(_tiltTempDisable || _wavedashState) && !_shieldDropState;

        TiltJoystick::setTiltState(_tiltOut);

        TiltJoystick::setXValue(_xAxis.getValue());
        TiltJoystick::setYValue(_yAxis.getValue());

        TiltJoystick::process();
    }

    void endCycle()
    {
        TiltJoystick::endCycle();
        _shieldDropState.endCycle();
        _tiltTempDisable.endCycle();
        _wavedashState.endCycle();
        _shieldDropNeutral.endCycle();
        _xAxis.endCycle();
        _yAxis.endCycle();
    }

    void setLsLeftState(const bool state)          { _xAxis.setLowState(state); }
    void setLsRightState(const bool state)         { _xAxis.setHighState(state); }
    void setLsDownState(const bool state)          { _yAxis.setLowState(state); }
    void setLsUpState(const bool state)            { _yAxis.setHighState(state); }
    void setXMod1State(const bool state)           { _xAxis.setMod1State(state); }
    void setXMod2State(const bool state)           { _xAxis.setMod2State(state); }
    void setYMod1State(const bool state)           { _yAxis.setMod1State(state); }
    void setYMod2State(const bool state)           { _yAxis.setMod2State(state); }
    void setTiltState(const bool state)            { _tiltState = state; }
    void setTiltTempDisableState(const bool state) { _tiltTempDisable.setActivatorState(state); }
    void setWavedashState(const bool state)        { _wavedashState.setActivatorState(state); }
    void setShieldDropState(const bool state)      { _shieldDropState = state; }

    void trimShieldDropDown()
    {
         _shieldDropValue -= 0.015;

        if (_shieldDropValue < -1.0)
            _shieldDropValue = -1.0;
    }

    void trimShieldDropUp()
    {
        _shieldDropValue += 0.015;

        if (_shieldDropValue > 0.0)
            _shieldDropValue = 0.0;
    }

    void resetShieldDrop() { _shieldDropValue = -0.67500; }

    void trimModsOutward()
    {
        _xAxis.trimModsOutward();
        _yAxis.trimModsOutward();
    }

    void trimModsInward()
    {
        _xAxis.trimModsInward();
        _yAxis.trimModsInward();
    }

    void resetMods()
    {
        _xAxis.resetMods();
        _yAxis.resetMods();
    }

private:
    bool _tiltState;
    bool _tiltOut;
    Activator _shieldDropState;
    TemporaryActivator _tiltTempDisable;
    TemporaryActivator _wavedashState;
    TemporaryActivator _shieldDropNeutral;

    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;

    double _shieldDropValue;
};

#endif // BUTTONONLYLEFTSTICK_H

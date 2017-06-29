#ifndef TILTJOYSTICK_H
#define TILTJOYSTICK_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class TiltJoystick : public Joystick
{
public:
    TiltJoystick()
    : Joystick(),
      _tiltState(false),
      _tiltAmount(0.60)
    {
        _tiltXOut.setTime(frames(7.0));
        _tiltYOut.setTime(frames(7.0));
    }

    virtual void process()
    {
        bool tiltResetX = _xValue.justActivated() || _xValue.justDeactivated() || _xValue.justCrossedInactiveRange();
        bool tiltResetY = _yValue.justActivated() || _yValue.justDeactivated() || _yValue.justCrossedInactiveRange();

        _tiltXOut.setActivatorState(tiltResetX);
        _tiltYOut.setActivatorState(tiltResetY);

        _tiltXOut.process();
        _tiltYOut.process();

        if (_tiltXOut && _tiltState)
            clampAxis(_xValue);

        if (_tiltYOut && _tiltState)
            clampAxis(_yValue);


        Joystick::setXValue(_xValue.getValue());
        Joystick::setYValue(_yValue.getValue());

        Joystick::process();
    }

    virtual void endCycle()
    {
        Joystick::endCycle();
        _tiltXOut.endCycle();
        _tiltYOut.endCycle();
        _xValue.endCycle();
        _yValue.endCycle();
    }

    virtual void setTiltState(const bool state) { _tiltState = state; }
    virtual void setXValue(const double value) { _xValue.setValue(value); }
    virtual void setYValue(const double value) { _yValue.setValue(value); }

private:
    bool _tiltState;

    TemporaryActivator _tiltXOut;
    TemporaryActivator _tiltYOut;

    BipolarControl _xValue;
    BipolarControl _yValue;

    const double _tiltAmount;

    void clampAxis(BipolarControl& axis)
    {
        double axisValue = axis.getValue();

        double lowClamp = -_tiltAmount;
        double highClamp = _tiltAmount;

        if (axisValue < lowClamp)
        {
            axis.setValue(lowClamp);
            return;
        }

        if (axisValue > highClamp)
        {
            axis.setValue(highClamp);
            return;
        }
    }
};


#endif // TILTJOYSTICK_H

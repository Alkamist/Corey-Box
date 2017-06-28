#ifndef TILTJOYSTICK_H
#define TILTJOYSTICK_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class TiltJoystick : public Joystick
{
public:
    TiltJoystick(const Activator& tilt, const BipolarControl& xControl, const BipolarControl& yControl)
    : Joystick(_xValue, _yValue),
      _xControl(xControl),
      _yControl(yControl),
      _tilt(tilt),
      _tiltXOut(frames(7.0), _tiltResetX),
      _tiltYOut(frames(7.0), _tiltResetY),
      _tiltAmount(0.60)
    {}

    virtual void process()
    {
        _xValue.setValue(_xControl.getValue());
        _yValue.setValue(_yControl.getValue());

        _tiltResetX.setState(_xControl.justActivated() || _xControl.justDeactivated() || _xControl.justCrossedInactiveRange());
        _tiltResetY.setState(_yControl.justActivated() || _yControl.justDeactivated() || _yControl.justCrossedInactiveRange());

        _tiltXOut.process();
        _tiltYOut.process();

        if (_tiltXOut && _tilt)
            clampAxis(_xValue);

        if (_tiltYOut && _tilt)
            clampAxis(_yValue);

        Joystick::process();
    }

    virtual void endCycle()
    {
        Joystick::endCycle();
        _xValue.endCycle();
        _yValue.endCycle();
        _tiltResetX.endCycle();
        _tiltResetY.endCycle();
        _tiltXOut.endCycle();
        _tiltYOut.endCycle();
    }

private:
    const BipolarControl& _xControl;
    const BipolarControl& _yControl;
    const Activator& _tilt;

    BipolarControl _xValue;
    BipolarControl _yValue;

    Activator _tiltResetX;
    Activator _tiltResetY;

    TemporaryActivator _tiltXOut;
    TemporaryActivator _tiltYOut;

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

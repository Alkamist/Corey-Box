#ifndef TILTJOYSTICK_H
#define TILTJOYSTICK_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

// This class represents a joystick that can be limited to not move
// fast enough to trigger dashing, rolling, spotdodging, etc...
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
        Joystick::process();

        bool tiltResetX = getXControl().justActivated() || getXControl().justDeactivated() || getXControl().justCrossedInactiveRange();
        bool tiltResetY = getYControl().justActivated() || getYControl().justDeactivated() || getYControl().justCrossedInactiveRange();

        _tiltXOut.setActivatorState(tiltResetX);
        _tiltYOut.setActivatorState(tiltResetY);

        _tiltXOut.process();
        _tiltYOut.process();

        if (_tiltXOut && _tiltState)
            Joystick::setXValue(clampValue(getXValue()));

        if (_tiltYOut && _tiltState)
            Joystick::setYValue(clampValue(getYValue()));

        Joystick::process();
    }

    virtual void endCycle()
    {
        Joystick::endCycle();
        _tiltXOut.endCycle();
        _tiltYOut.endCycle();
    }

    virtual void setTiltState(const bool state)  { _tiltState = state; }
    virtual void setXValue(const Control& value) { Joystick::setXValue(value); }
    virtual void setYValue(const Control& value) { Joystick::setYValue(value); }

private:
    bool _tiltState;

    TemporaryActivator _tiltXOut;
    TemporaryActivator _tiltYOut;

    const float _tiltAmount;

    const float clampValue(const float value) const
    {
        float lowClamp = -_tiltAmount;
        float highClamp = _tiltAmount;

        if (value < lowClamp)
            return lowClamp;

        if (value > highClamp)
            return highClamp;

        return 0.0;
    }
};


#endif // TILTJOYSTICK_H

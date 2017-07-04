#ifndef JOYSTICKBACKDASHFIXER_H
#define JOYSTICKBACKDASHFIXER_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class JoystickBackdashFixer
{
public:
    JoystickBackdashFixer()
    : _backdashFixDisable(false)
    {
        _killXTilt.setTime(frames(1.0));
    }

    void process(Joystick& joystick)
    {
        _killXTilt.setActivatorState(joystick.getDeadZone().checkIfInRange(joystick.getXMagnitude()));
        _killXTilt.process();

        if (_killXTilt && !_backdashFixDisable)
        {
            if (joystick.getTiltZone().checkIfInRange(joystick.getXMagnitude()))
                joystick.setXValue(clampValue(joystick.getXValue(), joystick.getDeadZone().getUpperBound()));
        }

        joystick.Joystick::process();
    }

    void endCycle()
    {
        _killXTilt.endCycle();
    }

    void setBackdashFixDisableState(const bool state) { _backdashFixDisable = state; }

private:
    bool _backdashFixDisable;
    TemporaryActivator _killXTilt;

    const float clampValue(const float value, const float clampValue) const
    {
        float lowClamp = -clampValue;
        float highClamp = clampValue;

        if (value < lowClamp)
            return lowClamp;

        if (value > highClamp)
            return highClamp;

        return value;
    }
};

#endif // JOYSTICKBACKDASHFIXER_H

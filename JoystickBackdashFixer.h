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
        _killXTilt.setTime(frames(1));
    }

    void process(FightingJoystick& joystick)
    {
        _killXTilt = joystick.xIsInDeadZone();
        _killXTilt.process();

        if (_killXTilt && !_backdashFixDisable)
        {
            if (joystick.xIsInTiltZone())
                joystick.xValue = clampValue(joystick.xValue, joystick.getDeadZoneUpperBound());
        }

        joystick.FightingJoystick::process();
    }

    void endCycle()
    {
        _killXTilt.endCycle();
    }

    void setBackdashFixDisableState(const bool state) { _backdashFixDisable = state; }

private:
    bool _backdashFixDisable;
    TemporaryActivator _killXTilt;

    const uint8_t clampValue(const uint8_t value, const uint8_t clampValue) const
    {
        uint8_t lowClamp = 128 - clampValue;
        uint8_t highClamp = 128 + clampValue;

        if (value < lowClamp)
            return lowClamp;

        if (value > highClamp)
            return highClamp;

        return value;
    }
};

#endif // JOYSTICKBACKDASHFIXER_H

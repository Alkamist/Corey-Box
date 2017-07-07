#ifndef JOYSTICKBACKDASHFIXER_H
#define JOYSTICKBACKDASHFIXER_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "TimedActivator.h"
#include "Frames.h"
#include "ClampBipolar.h"

class JoystickBackdashFixer
{
public:
    JoystickBackdashFixer()
    : _backdashFixDisable(false)
    {
        // You have to hold down for a certain period of time to get into crouch.
        _inCrouch.setTime(frames(5));

        _killCrouchGetup.setTime(frames(1));
        _killXTilt.setTime(frames(1));
    }

    void process(FightingJoystick& joystick)
    {
        _inCrouch = joystick.yIsInCrouchZone();
        _inCrouch.process();

        _killCrouchGetup = _inCrouch.justDeactivated();
        _killCrouchGetup.process();

        _killXTilt = joystick.xIsInDeadZone() || joystick.xValue.justCrossedCenter();
        _killXTilt.process();

        if (_killXTilt && !_backdashFixDisable)
            if (joystick.xIsInTiltZone())
                joystick.xValue = clampBipolar(joystick.xValue, joystick.getDeadZoneUpperBound());

        if (_killCrouchGetup && !_backdashFixDisable)
            if (!joystick.yIsInCrouchZone())
                joystick.yValue = joystick.getCrouchZoneUpperBound();

        joystick.FightingJoystick::process();
    }

    void endCycle()
    {
        _inCrouch.endCycle();
        _killCrouchGetup.endCycle();
        _killXTilt.endCycle();
    }

    void setBackdashFixDisableState(const bool state) { _backdashFixDisable = state; }

private:
    bool _backdashFixDisable;

    TimedActivator _inCrouch;
    TemporaryActivator _killCrouchGetup;
    TemporaryActivator _killXTilt;
};

#endif // JOYSTICKBACKDASHFIXER_H

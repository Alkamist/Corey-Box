#ifndef ANALOGLEFTSTICK_H
#define ANALOGLEFTSTICK_H

#include "Joystick.h"
#include "TemporaryActivator.h"
#include "Frames.h"

class AnalogLeftStick : public Joystick
{
public:
    AnalogLeftStick()
    : Joystick(),
      _shieldDropValue(-0.67500),
      _backdashFixOverride(false)
    {
        _killXTilt.setTime(frames(1.0));
    }

    void process()
    {
        Joystick::process();

        if (!_backdashFixOverride)
            fixBackdash();

        Joystick::process();
    }

    void endCycle()
    {
        Joystick::endCycle();
        _killXTilt.endCycle();
        _preventCrouchGetup.endCycle();
    }

    void setBackdashFixOverride(const bool state) { _backdashFixOverride = state; }

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

private:
    float _shieldDropValue;

    bool _backdashFixOverride;

    TemporaryActivator _killXTilt;
    TemporaryActivator _preventCrouchGetup;

    void fixBackdash()
    {
        float xValue = getXValue();
        float yValue = getYValue();
        float xMagnitude = getXMagnitude();

        bool xIsInDeadRange = xMagnitude <= 0.2750;
        bool xIsInTiltRange = xMagnitude > 0.2750 && xMagnitude <= 0.7875;
        bool xIsInSmashRange = xMagnitude > 0.8000;
        bool yIsInCrouchRange = yValue <= -0.6125;

        _killXTilt.setActivatorState(xIsInDeadRange);
        _killXTilt.process();

        if (_killXTilt)
        {
            if (xIsInSmashRange)
                return;

            if (xIsInTiltRange)
                setXValue(clampValue(xValue, 0.2750));

            _preventCrouchGetup.setActivatorState(yIsInCrouchRange && xIsInTiltRange);
            _preventCrouchGetup.process();

            if (_preventCrouchGetup && !yIsInCrouchRange)
                setXValue(-0.6125);
        }
    }

    const float clampValue(const float value, const float clampValue)
    {
        float lowClamp = -clampValue;
        float highClamp = clampValue;

        if (value < lowClamp)
            return lowClamp;

        if (value > highClamp)
            return highClamp;

        return 0.0;
    }
};

#endif // ANALOGLEFTSTICK_H

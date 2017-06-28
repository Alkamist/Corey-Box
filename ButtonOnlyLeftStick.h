#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "TiltJoystick.h"
#include "DoubleModAxis.h"
#include "Signum.h"

class ButtonOnlyLeftStick : public TiltJoystick
{
public:
    ButtonOnlyLeftStick(const Activator& lsLeft, const Activator& lsRight,
                        const Activator& lsDown, const Activator& lsUp,
                        const Activator& xMod1, const Activator& xMod2,
                        const Activator& yMod1, const Activator& yMod2,
                        const Activator& tilt, const Activator& wavedash,
                        const Activator& shieldDrop)
    : TiltJoystick(_tiltOut, _xAxis, _yAxis),
      _tilt(tilt),
      _shieldDrop(shieldDrop),
      _lsDown(lsDown),
      _xAxis(lsLeft, lsRight, xMod1, xMod2),
      _yAxis(_lsDownOut, lsUp, yMod1, yMod2),
      _shieldDropOut(frames(1.0), shieldDrop),
      _wavedashOut(frames(5.0), wavedash),
      _shieldDropValue(-0.67500)
    {}

    virtual void process()
    {
        _shieldDropOut.process();
        _wavedashOut.process();

        _lsDownOut.setState(_lsDown || _wavedashOut);

        _xAxis.process();
        _yAxis.process();

        if (_shieldDropOut && _shieldDrop)
        {
            _xAxis.setValue(0.0);
            _yAxis.setValue(0.0);
        }

        if (!_shieldDropOut && _shieldDrop)
        {
            _xAxis.setValue(0.0);
            _yAxis.setValue(_shieldDropValue);
        }

        _tiltOut.setState(_tilt && !_wavedashOut && !_shieldDrop);

        TiltJoystick::process();
    }

    virtual void endCycle()
    {
        TiltJoystick::endCycle();
        _xAxis.endCycle();
        _yAxis.endCycle();
        _shieldDropOut.endCycle();
        _wavedashOut.endCycle();
        _tiltOut.endCycle();
        _lsDownOut.endCycle();
    }

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
    const Activator& _tilt;
    const Activator& _shieldDrop;
    const Activator& _lsDown;

    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;

    Activator _tiltOut;
    Activator _lsDownOut;

    TemporaryActivator _shieldDropOut;
    TemporaryActivator _wavedashOut;

    double _shieldDropValue;
};

#endif // BUTTONONLYLEFTSTICK_H

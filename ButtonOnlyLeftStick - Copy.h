#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "TiltJoystick.h"
#include "DoubleModAxis.h"
#include "Signum.h"
#include "TemporaryActivator.h"

class ButtonOnlyLeftStick : public TiltJoystick
{
public:
    ButtonOnlyLeftStick()
    : TiltJoystick(),
      _shieldDropReset(frames(1)),
      _tiltTempDisable(frames(5))
    {}

    virtual void update()
    {
        TiltJoystick::update();
        _xAxis.update();
        _yAxis.update();
        _shieldDropReset.update();
        _tiltTempDisable.update();
    }

    virtual void setControls(const bool left, const bool right,
                             const bool down, const bool up,
                             const bool xMod1, const bool xMod2,
                             const bool yMod1, const bool yMod2,
                             const bool tilt, const bool tiltTempDisable,
                             const bool shieldDrop)
    {
        _shieldDropReset.setControls(shieldDrop);

        _xAxis.setControls(left, right, xMod1, xMod2);
        _yAxis.setControls(down, up, yMod1, yMod2);

        if (_shieldDropReset && shieldDrop)
        {
            _xAxis = 0.0;
            _yAxis = 0.0;
        }

        if (!_shieldDropReset && shieldDrop)
        {
            _xAxis = 0.0;
            _yAxis = -0.67500;
        }

        _tiltTempDisable.setControls(tiltTempDisable);

        bool tiltShouldHappen = tilt && !_tiltTempDisable && !shieldDrop;

        TiltJoystick::setControls(_xAxis, _yAxis, tiltShouldHappen);
    }

private:
    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;

    TemporaryActivator _shieldDropReset;
    TemporaryActivator _tiltTempDisable;
};

#endif // BUTTONONLYLEFTSTICK_H
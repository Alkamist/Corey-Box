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
      _tiltTempDisable(Frames(5, 60).getMillis())
    {}

    virtual void update()
    {
        TiltJoystick::update();
        _xAxis.update();
        _yAxis.update();
        _tiltTempDisable.update();
    }

    virtual void setControls(const bool left, const bool right,
                             const bool down, const bool up,
                             const bool xMod1, const bool xMod2,
                             const bool yMod1, const bool yMod2,
                             const bool tilt, const bool tiltTempDisable,
                             const bool shieldDrop)
    {
        _xAxis.setControls(left, right, xMod1, xMod2);
        _yAxis.setControls(down, up, yMod1, yMod2);

        _tiltTempDisable.setControls(tiltTempDisable);

        bool tiltShouldHappen = tilt && !_tiltTempDisable && !shieldDrop;

        TiltJoystick::setControls(_xAxis, _yAxis, tiltShouldHappen);
    }

private:
    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;

    TemporaryActivator _tiltTempDisable;
};

#endif // BUTTONONLYLEFTSTICK_H
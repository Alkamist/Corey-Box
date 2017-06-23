#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "TiltJoystick.h"
#include "DoubleModAxis.h"
#include "Signum.h"
#include "TemporaryActivator.h"

class ButtonOnlyLeftStick : public TiltJoystick
{
public:
    explicit ButtonOnlyLeftStick()
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

        bool upLeft = up && left;
        bool upRight = up && right;
        bool downLeft = down && left;
        bool downRight = down && right;

        bool anyAngle = upLeft || upRight || downLeft || downRight;

        bool anyXMod = xMod1 || xMod2;
        bool anyYMod = yMod1 || yMod2;

        if (anyAngle && anyXMod && !anyYMod)
            calculateOtherAxis(_xAxis, _yAxis);

        if (anyAngle && anyYMod && !anyXMod)
            calculateOtherAxis(_yAxis, _xAxis);

        _tiltTempDisable.setControls(tiltTempDisable);

        bool tiltShouldHappen = tilt && !_tiltTempDisable && !shieldDrop;

        TiltJoystick::setControls(_xAxis, _yAxis, tiltShouldHappen);
    }

private:
    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;

    TemporaryActivator _tiltTempDisable;

    void calculateOtherAxis(Control& axisToSet, Control& otherAxis)
    {
        double valueSign = signum(otherAxis.getValue());
        double axisMagnitude = sqrt(1.0 - square(axisToSet.getValue()));
        double newValue = valueSign * axisMagnitude;
        otherAxis = newValue;
    }
};

#endif // BUTTONONLYLEFTSTICK_H
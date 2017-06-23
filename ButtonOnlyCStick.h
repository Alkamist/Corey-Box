#ifndef BUTTONONLYCSTICK_H
#define BUTTONONLYCSTICK_H

#include "Joystick.h"
#include "SingleModAxis.h"
#include "TwoButtonControl.h"

class ButtonOnlyCStick : public Joystick
{
public:
    explicit ButtonOnlyCStick()
    : Joystick()
    {}

    virtual void update()
    {
        Joystick::update();
        _xAxis.update();
        _yAxis.update();
    }

    virtual void setControls(const bool left, const bool right,
                             const bool down, const bool up,
                             const Joystick& leftStick)
    {
        bool skewYAxis = leftStick.getYControl().isActive();

        _xAxis.setControls(left, right);
        _yAxis.setControls(down, up, skewYAxis);

        Joystick::setControls(_xAxis, _yAxis);
    }

private:
    TwoButtonControl _xAxis;
    SingleModAxis _yAxis;
};

#endif // BUTTONONLYCSTICK_H
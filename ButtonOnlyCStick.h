#ifndef BUTTONONLYCSTICK_H
#define BUTTONONLYCSTICK_H

#include "Joystick.h"
#include "SingleModAxis.h"
#include "TwoButtonControl.h"

class ButtonOnlyCStick : public Joystick
{
public:
    ButtonOnlyCStick()
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
                             const bool lsDown, const bool lsUp,
                             const bool tilt)
    {
        bool skewCondition = (tilt && (lsDown || lsUp)) && (left || right) && !(down || up);

        bool skewDown = lsDown && skewCondition;
        bool skewUp = lsUp && skewCondition;

        bool cDown = down || skewDown;
        bool cUp = up || skewUp;

        _xAxis.setControls(left, right);
        _yAxis.setControls(cDown, cUp, skewCondition);

        Joystick::setControls(_xAxis, _yAxis);
    }

private:
    TwoButtonControl _xAxis;
    SingleModAxis _yAxis;
};

#endif // BUTTONONLYCSTICK_H
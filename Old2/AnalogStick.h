#ifndef ANALOGSTICK_H
#define ANALOGSTICK_H

#include "AnalogAxis.h"

class AnalogStick
{
public:
    AnalogStick() {}

    void update();

    void setDeadZone(double deadZone);

    inline AnalogAxis& getXAxis() { return _xAxis; }
    inline AnalogAxis& getYAxis() { return _yAxis; }

private:
    AnalogAxis _xAxis;
    AnalogAxis _yAxis;
};



void AnalogStick::update()
{
    _xAxis.update();
    _yAxis.update();
}

void AnalogStick::setDeadZone(double deadZone)
{
    _xAxis.setDeadZone(deadZone);
    _yAxis.setDeadZone(deadZone);
}

#endif // ANALOGSTICK_H
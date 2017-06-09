#ifndef ANALOGAXIS_H
#define ANALOGAXIS_H

#include "ControlValue.h"
#include "DeadZone.h"

// This class is just a ControlValue that defaults to the
// default values you would expect from a perfect joystick axis.
class AnalogAxis : public ControlValue
{
public:
    AnalogAxis();

    void init();
};



void AnalogAxis::init()
{
    DeadZone defaultDeadZone;
    defaultDeadZone.setCenter(0.5);
    defaultDeadZone.setRange(0.1);

    ControlValue::setDeadZone(defaultDeadZone);
    ControlValue::resetValue();
}

AnalogAxis::AnalogAxis()
: ControlValue()
{
    init();
}

#endif // ANALOGAXIS_H
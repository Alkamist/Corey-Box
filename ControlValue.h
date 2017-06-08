#ifndef CONTROLVALUE_H
#define CONTROLVALUE_H

#include "UpdatedValue.h"
#include "DeadZone.h"

// This class is the base representation of a value that
// can control things. This could represent a button, an
// axis, or something else perhaps. It has a deadzone that
// can be set that affects when the value is active or not.
class ControlValue : public UpdatedValue<double>
{
public:
    ControlValue();

    operator uint8_t() { return getValue() * 255; }

    void setValue(double value);
    void setValue(bool value);
    void setDeadZone(DeadZone deadZone);

    bool isActive();
    bool justActivated();
    bool justDeactivated();

private:
    DeadZone _deadZone;

    void applyDeadZone();
};



void ControlValue::setValue(double value)
{
    UpdatedValue<double>::setValue(value);

    applyDeadZone();
}

void ControlValue::setValue(bool value)
{
    if (value)
        setValue(1.0);
    else
        setValue(0.0);
}

void ControlValue::setDeadZone(DeadZone deadZone)
{
    _deadZone = deadZone;

    applyDeadZone();
}

bool ControlValue::isActive()
{
    if (!_deadZone.check(getValue()))
        return true;

    return false;
}

bool ControlValue::justActivated()
{
    if (!_deadZone.check(getValue()) && _deadZone.check(getPreviousValue()))
        return true;

    return false;
}

bool ControlValue::justDeactivated()
{
    if (_deadZone.check(getValue()) && !_deadZone.check(getPreviousValue()))
        return true;

    return false;
}

void ControlValue::applyDeadZone()
{
    if (_deadZone.check(getValue()))
        UpdatedValue<double>::setValue(_deadZone.getCenter());
}

ControlValue::ControlValue()
: UpdatedValue<double>(0.0)
{
    _deadZone.setCenter(0.0);
    _deadZone.setRange(0.0);
}

#endif // CONTROLVALUE_H
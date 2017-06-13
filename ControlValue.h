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
    explicit ControlValue(const double value);
    explicit ControlValue(const double value, const double center);

    ControlValue operator=(const double value);
    ControlValue operator=(const bool value);

    operator uint8_t() { return getValue() * 255; }
    operator double()  { return getValue(); }
    operator bool()    { return isActive(); }

    void initDeadZone(const double value);
    void resetValue()                    { setValue(_deadZone.getCenter()); }

    void setValue(const double value);
    void setValue(const bool value);
    const double getDefaultValue() const { return _deadZone.getCenter(); }

    void setDeadZone(const DeadZone deadZone);
    DeadZone getDeadZone() const         { return _deadZone; }

    const bool isActive() const;
    const bool justActivated() const;
    const bool justDeactivated() const;

private:
    DeadZone _deadZone;

    void applyDeadZone();
};



void ControlValue::initDeadZone(const double center)
{
    _deadZone.setCenter(center);
    _deadZone.setRange(0.1);
}

void ControlValue::setValue(const double value)
{
    UpdatedValue<double>::setValue(value);

    applyDeadZone();
}

void ControlValue::setValue(const bool value)
{
    if (value)
        setValue(1.0);
    else
        setValue(0.0);
}

void ControlValue::setDeadZone(const DeadZone deadZone)
{
    _deadZone = deadZone;

    applyDeadZone();
}

const bool ControlValue::isActive() const
{
    if (!_deadZone.check(getValue()))
        return true;

    return false;
}

const bool ControlValue::justActivated() const
{
    if (!_deadZone.check(getValue()) && _deadZone.check(getPreviousValue()))
        return true;

    return false;
}

const bool ControlValue::justDeactivated() const
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
    initDeadZone(0.0);
}

ControlValue::ControlValue(const double value)
: UpdatedValue<double>(value)
{
    initDeadZone(0.0);
}

ControlValue::ControlValue(const double value, const double center)
: UpdatedValue<double>(value)
{
    initDeadZone(center);
}

ControlValue ControlValue::operator=(const double value)
{
    setValue(value);

    return *this;
}

ControlValue ControlValue::operator=(const bool value)
{
    setValue(value);

    return *this;
}

#endif // CONTROLVALUE_H
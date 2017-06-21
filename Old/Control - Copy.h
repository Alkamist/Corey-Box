#ifndef CONTROL_H
#define CONTROL_H

#include "ControlState.h"
#include "Range.h"
#include "UpdatedValue.h"

class Control : public UpdatedValue<double>
{
public:
    Control();
    explicit Control(const double value);
    explicit Control(const bool value);

    virtual void update();

    void setValue(const double value);
    void setValue(const bool value);

    const Range& getRange() const;

    const bool stateHasChanged() const;
    const bool isActive() const;
    const bool justActivated() const;
    const bool justDeactivated() const;

    const unsigned int getStateTime() const;

    operator bool() const;

    const bool operator =(const bool value);
    const bool operator !();

    const bool operator &&(const bool value) const;
    const bool operator ||(const bool value) const;
    const bool operator ==(const bool value) const;
    const bool operator !=(const bool value) const;

private:
    ControlState _state;

    Range _inactiveRange;
    Range _valueRange;
};



void Control::update()
{
    UpdatedValue<double>::update();

    _state.update();
}

void Control::setValue(const double value)
{
    UpdatedValue<double>::setValue(_valueRange.enforceRange(value));

    bool valueIsActive = !_inactiveRange.checkIfInRange(getValue());
    _state = valueIsActive;
}

void Control::setValue(const bool value)
{
    if (value == true)
    {
        UpdatedValue<double>::setValue(1.0);
        _state = true;
    }

    if (value == false)
    {
        UpdatedValue<double>::setValue(0.0);
        _state = false;
    }
}

const Range& Control::getRange() const
{
    return _valueRange;
}

const bool Control::stateHasChanged() const
{
    return _state.hasChanged();
}

const bool Control::isActive() const
{
    return _state;
}

const bool Control::justActivated() const
{
    return _state.justActivated();
}

const bool Control::justDeactivated() const
{
    return _state.justDeactivated();
}

const unsigned int Control::getStateTime() const
{
    return _state.timeSinceChange();
}

Control::Control()
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude(0.0, 0.2)),
  _valueRange(Bounds(0.0, 1.0))
{
    setValue(0.0);
}

Control::Control(const double value)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude(0.0, 0.2)),
  _valueRange(Bounds(0.0, 1.0))
{
    setValue(value);
}

Control::Control(const bool value)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude(0.0, 0.2)),
  _valueRange(Bounds(0.0, 1.0))
{
    setValue(value);
}

Control::operator bool() const
{
    return _state;
}

const bool Control::operator =(const bool value)
{
    setValue(value);
    return _state;
}

const bool Control::operator !()
{
    return !_state;
}

const bool Control::operator &&(const bool value) const
{
    return _state && value;
}

const bool Control::operator ||(const bool value) const
{
    return _state || value;
}

const bool Control::operator ==(const bool value) const
{
    return _state == value;
}

const bool Control::operator !=(const bool value) const
{
    return _state != value;
}

#endif // CONTROL_H
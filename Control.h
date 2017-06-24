#ifndef CONTROL_H
#define CONTROL_H

#include "ControlState.h"
#include "Range.h"
#include "UpdatedValue.h"
#include "Rescale.h"

class Control : public UpdatedValue<double>
{
public:
    Control();
    explicit Control(const double value);
    explicit Control(const bool value);
    explicit Control(const double value, const Range<double>& valueRange);
    explicit Control(const double value, const Range<double>& valueRange, const Range<double>& inactiveRange);

    virtual void update();

    void setValue(const Control& value);
    void setValue(const double value);
    void setValue(const bool value);

    const void setValueRange(const Range<double>& range) { _valueRange = range; }
    const Range<double>& getValueRange() const           { return _valueRange; }


    const bool hasCrossedInactiveRange() const;
    const bool stateHasChanged() const                   { return _state.hasChanged(); }
    const bool isActive() const                          { return _state.getValue(); }
    const bool justActivated() const                     { return _state.justActivated(); }
    const bool justDeactivated() const                   { return _state.justDeactivated(); }

    //=================== OPERATORS ===================

    virtual const Control& operator =(const Control& value);
    virtual const Control& operator =(const double value);
    virtual const Control& operator =(const bool value);
    virtual operator double() const;
    virtual operator bool() const;
    virtual const double operator +() const;
    virtual const double operator -() const;
    virtual const bool operator !() const;
    virtual const bool operator &&(const bool value) const;
    virtual const bool operator ||(const bool value) const;
    virtual const bool operator ==(const bool value) const;
    virtual const bool operator !=(const bool value) const;
    virtual const bool operator >(const double value) const;
    virtual const bool operator <(const double value) const;
    virtual const bool operator >=(const double value) const;
    virtual const bool operator <=(const double value) const;

private:
    ControlState _state;

    Range<double> _inactiveRange;
    Range<double> _valueRange;
};



void Control::update()
{
    UpdatedValue<double>::update();
    _state.update();
}

void Control::setValue(const Control& value)
{
    double rescaledValue = Rescale::rescale<double>(value.getValue(), value.getValueRange(), _valueRange);

    UpdatedValue<double>::setValue(rescaledValue);

    bool valueIsActive = !_inactiveRange.checkIfInRange(getValue());
    _state = valueIsActive;
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
        UpdatedValue<double>::setValue(_valueRange.enforceRange(1.0));
        _state = true;
    }

    if (value == false)
    {
        UpdatedValue<double>::setValue(_valueRange.enforceRange(0.0));
        _state = false;
    }
}

const bool Control::hasCrossedInactiveRange() const
{
    bool crossedHighToLow = getPreviousValue() > _inactiveRange.getUpperBound()
                         && getValue() < _inactiveRange.getLowerBound();

    bool crossedLowToHigh = getPreviousValue() < _inactiveRange.getLowerBound()
                         && getValue() > _inactiveRange.getUpperBound();

    return crossedHighToLow || crossedLowToHigh;
}

Control::Control()
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(Bounds<double>(0.0, 1.0))
{
    setValue(0.0);
    update();
}

Control::Control(const double value)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(Bounds<double>(0.0, 1.0))
{
    setValue(value);
    update();
}

Control::Control(const bool value)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(Bounds<double>(0.0, 1.0))
{
    setValue(value);
    update();
}

Control::Control(const double value, const Range<double>& valueRange)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(valueRange)
{
    setValue(value);
    update();
}

Control::Control(const double value, const Range<double>& valueRange, const Range<double>& inactiveRange)
: UpdatedValue<double>(0.0),
  _inactiveRange(inactiveRange),
  _valueRange(valueRange)
{
    setValue(value);
    update();
}

//=================== OPERATORS ===================

const Control& Control::operator =(const Control& value)
{
    setValue(value);
    return *this;
}

const Control& Control::operator =(const double value)
{
    setValue(value);
    return *this;
}

const Control& Control::operator =(const bool value)
{
    setValue(value);
    return *this;
}

Control::operator double() const
{
    return getValue();
}

Control::operator bool() const
{
    return isActive();
}

const double Control::operator +() const
{
    return getValue();
}

const double Control::operator -() const
{
    return -getValue();
}

const bool Control::operator !() const
{
    return !isActive();
}

const bool Control::operator &&(const bool value) const
{
    return isActive() && value;
}

const bool Control::operator ||(const bool value) const
{
    return isActive() || value;
}

const bool Control::operator ==(const bool value) const
{
    return isActive() == value;
}

const bool Control::operator !=(const bool value) const
{
    return isActive() != value;
}

const bool Control::operator >(const double value) const
{
    return getValue() > value;
}

const bool Control::operator <(const double value) const
{
    return getValue() < value;
}

const bool Control::operator >=(const double value) const
{
    return getValue() >= value;
}

const bool Control::operator <=(const double value) const
{
    return getValue() <= value;
}

#endif // CONTROL_H
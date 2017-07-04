#ifndef CONTROL_H
#define CONTROL_H

#include "Activator.h"
#include "Range.h"

class Control : public Activator
{
public:
    Control();

    virtual void endCycle();

    virtual void setValue(const Control& value);
    virtual void setValue(const double value);
    virtual void setValue(const float value);
    virtual void setValue(const bool value);

    const float getValue() const                       { return _currentValue; }
    const float getPreviousValue() const               { return _previousValue; }

    void setValueRange(const Range<float>& range)      { _valueRange = range; }
    const Range<float>& getValueRange() const          { return _valueRange; }

    const bool valueJustChanged() const                { return _currentValue != _previousValue; }

    const bool justCrossedInactiveRange() const;

    void makeUnipolar();
    void makeBipolar();

    const Control& operator =(const Control& value)    { setValue(value); return *this; }
    const Control& operator =(const Activator& value)  { setValue(value); return *this; }
    const Control& operator =(const double value)      { setValue(value); return *this; }
    const Control& operator =(const float value)       { setValue(value); return *this; }
    const Control& operator =(const bool value)        { setValue(value); return *this; }
    operator float() const                             { return getValue(); }

    const float operator +(const Control& value)       { return getValue() + _valueRange.rescaleValue(value, value.getValueRange()); }
    const Control& operator +=(const Control& value)   { setValue(getValue() + _valueRange.rescaleValue(value, value.getValueRange())); return *this; }
    const float operator -(const Control& value)       { return getValue() - _valueRange.rescaleValue(value, value.getValueRange()); }
    const Control& operator -=(const Control& value)   { setValue(getValue() - _valueRange.rescaleValue(value, value.getValueRange())); return *this; }
    const float operator *(const Control& value)       { return getValue() * _valueRange.rescaleValue(value, value.getValueRange()); }
    const Control& operator *=(const Control& value)   { setValue(getValue() * _valueRange.rescaleValue(value, value.getValueRange())); return *this; }
    const float operator /(const Control& value)       { return getValue() / _valueRange.rescaleValue(value, value.getValueRange()); }
    const Control& operator /=(const Control& value)   { setValue(getValue() / _valueRange.rescaleValue(value, value.getValueRange())); return *this; }

    const float operator +(const float value)          { return getValue() + value; }
    const Control& operator +=(const float value)      { setValue(getValue() + value); return *this; }
    const float operator -(const float value)          { return getValue() - value; }
    const Control& operator -=(const float value)      { setValue(getValue() - value); return *this; }
    const float operator *(const float value)          { return getValue() * value; }
    const Control& operator *=(const float value)      { setValue(getValue() * value); return *this; }
    const float operator /(const float value)          { return getValue() / value; }
    const Control& operator /=(const float value)      { setValue(getValue() / value); return *this; }

    const bool operator ==(const Control& value) const { return getValue() == _valueRange.rescaleValue(value, value.getValueRange()); }
    const bool operator !=(const Control& value) const { return getValue() != _valueRange.rescaleValue(value, value.getValueRange()); }
    const bool operator <(const Control& value) const  { return getValue() < _valueRange.rescaleValue(value, value.getValueRange()); }
    const bool operator <=(const Control& value) const { return getValue() <= _valueRange.rescaleValue(value, value.getValueRange()); }
    const bool operator >(const Control& value) const  { return getValue() > _valueRange.rescaleValue(value, value.getValueRange()); }
    const bool operator >=(const Control& value) const { return getValue() >= _valueRange.rescaleValue(value, value.getValueRange()); }

    const bool operator ==(const float value) const    { return getValue() == value; }
    const bool operator !=(const float value) const    { return getValue() != value; }
    const bool operator <(const float value) const     { return getValue() < value; }
    const bool operator <=(const float value) const    { return getValue() <= value; }
    const bool operator >(const float value) const     { return getValue() > value; }
    const bool operator >=(const float value) const    { return getValue() >= value; }

private:
    float _currentValue;
    float _previousValue;

    Range<float> _inactiveRange;
    Range<float> _valueRange;
};



void Control::endCycle()
{
    Activator::endCycle();
    _previousValue = _currentValue;
}

void Control::setValue(const Control& value)
{
    _currentValue = _valueRange.rescaleValue(value, value.getValueRange());

    bool valueIsActive = !_inactiveRange.checkIfInRange(getValue());
    Activator::setState(valueIsActive);
}

void Control::setValue(const double value)
{
    setValue(static_cast<float>(value));
}

void Control::setValue(const float value)
{
    _currentValue = _valueRange.enforceRange(value);

    bool valueIsActive = !_inactiveRange.checkIfInRange(getValue());
    Activator::setState(valueIsActive);
}

void Control::setValue(const bool value)
{
    if (value == true)
    {
        _currentValue = _valueRange.getUpperBound();
        Activator::setState(true);
    }

    if (value == false)
    {
        _currentValue = _inactiveRange.getCenter();
        Activator::setState(false);
    }
}

const bool Control::justCrossedInactiveRange() const
{
    bool crossedHighToLow = _previousValue > _inactiveRange.getUpperBound()
                         && _currentValue < _inactiveRange.getLowerBound();

    bool crossedLowToHigh = _previousValue < _inactiveRange.getLowerBound()
                         && _currentValue > _inactiveRange.getUpperBound();

    return crossedHighToLow || crossedLowToHigh;
}

void Control::makeUnipolar()
{
    Range<float> oldRange(_valueRange);

    _valueRange.setLowerBound(0.0);
    _valueRange.setUpperBound(1.0);
    _inactiveRange.setCenter(0.0);
    _inactiveRange.setMagnitude(0.2);

    _currentValue = _valueRange.rescaleValue(_currentValue, oldRange);
    _previousValue = _valueRange.rescaleValue(_previousValue, oldRange);
}

void Control::makeBipolar()
{
    Range<float> oldRange(_valueRange);

    _valueRange.setLowerBound(-1.0);
    _valueRange.setUpperBound(1.0);
    _inactiveRange.setCenter(0.0);
    _inactiveRange.setMagnitude(0.2);

    _currentValue = _valueRange.rescaleValue(_currentValue, oldRange);
    _previousValue = _valueRange.rescaleValue(_previousValue, oldRange);
}

Control::Control()
: Activator(false),
  _currentValue(0.0),
  _previousValue(0.0),
  _inactiveRange(Range<float>(CenterAndMagnitude<float>(0.0, 0.2))),
  _valueRange(Range<float>(Bounds<float>(0.0, 1.0)))
{
    setValue(0.0);
}

#endif // CONTROL_H
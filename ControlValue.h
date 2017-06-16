#ifndef CONTROLVALUE_H
#define CONTROLVALUE_H

#include "UpdatedValue.h"
#include "Range.h"
#include "ValueScaler.h"

// A ControlValue is the base class of any value used to control things.
// Its value ranges from 0 to 1 and it keeps track of various useful traits.
class ControlValue : public UpdatedValue<double>
{
public:
    ControlValue();
    explicit ControlValue(const double value);
    explicit ControlValue(const double value, const Range& inactiveRange);

    virtual void update();

    virtual void setValue(double value);
    virtual void setValue(const bool value);

    const double getDefaultValue() const               { return _inactiveRange.getCenter(); }

    void setInactiveRange(const Range& inactiveRange)  { _inactiveRange = inactiveRange; }

    const bool isActive() const                        { return _isActive.getValue(); }
    const bool justActivated() const                   { return _isActive.hasChanged() && _isActive.getValue(); }
    const bool justDeactivated() const                 { return _isActive.hasChanged() && !_isActive.getValue(); }
    const unsigned int getTimeSinceStateChange() const { return _isActive.timeSinceChange(); }

private:
    UpdatedValue<bool> _isActive;
    UpdatedValue<double> _rawValue;

    Range _inactiveRange;
    Range _valueRange;

    void setFromRawValue(const double value);
    const double calculateScaledValue(const double value);
};




void ControlValue::update()
{
    UpdatedValue<double>::update();

    _isActive.update();
    _rawValue.update();
}

void ControlValue::setValue(double value)
{
    _valueRange.enforceRange(value);

    setFromRawValue(value);
}

void ControlValue::setValue(const bool value)
{
    if (value == true)
        setFromRawValue(1.0);
    else
        setFromRawValue(0.0);
}

void ControlValue::setFromRawValue(const double value)
{
    _isActive = !_inactiveRange.checkIfInRange(value);
    _rawValue = value;

    double rescaledValue = calculateScaledValue(value);
    UpdatedValue<double>::setValue(rescaledValue);
}

const double ControlValue::calculateScaledValue(const double value)
{
    double defaultValue = _inactiveRange.getCenter();

    if (value < defaultValue)
    {
        double boundOfInterest = _inactiveRange.getLowerBound();

        Range oldRange(Bounds(boundOfInterest, 0.0));
        Range newRange(Bounds(defaultValue, 0.0));

        ValueScaler valueScaler(oldRange, newRange);

        return valueScaler.rescaleValue(value);
    }

    if (value > defaultValue)
    {
        double boundOfInterest = _inactiveRange.getUpperBound();

        Range oldRange(Bounds(boundOfInterest, 1.0));
        Range newRange(Bounds(defaultValue, 1.0));

        ValueScaler valueScaler(oldRange, newRange);

        return valueScaler.rescaleValue(value);
    }

    return defaultValue;
}

ControlValue::ControlValue()
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude(0.0, 0.2)),
  _valueRange(Bounds(0.0, 1.0)),
  _rawValue(0.0),
  _isActive(false)
{}

ControlValue::ControlValue(const double value)
: UpdatedValue<double>(value),
  _inactiveRange(CenterAndMagnitude(0.0, 0.2)),
  _valueRange(Bounds(0.0, 1.0)),
  _rawValue(0.0),
  _isActive(false)
{}

ControlValue::ControlValue(const double value, const Range& inactiveRange)
: UpdatedValue<double>(value),
  _inactiveRange(inactiveRange),
  _valueRange(Bounds(0.0, 1.0)),
  _rawValue(value),
  _isActive(false)
{}

#endif // CONTROLVALUE_H
#ifndef CONTROLVALUE_H
#define CONTROLVALUE_H

#include "UpdatedValue.h"
#include "Range.h"

// A ControlValue is the base class of any value used to control things.
// You can define its inactive range, and it will keep track of various
// helpful traits with respect to that range.
class ControlValue : public UpdatedValue<double>
{
public:
    ControlValue();
    explicit ControlValue(const double value);
    explicit ControlValue(const double value, const Range& inactiveRange);
    explicit ControlValue(const double value, const Range& inactiveRange, const Range& valueRange);

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

    Range _inactiveRange;
    Range _valueRange;

    void inputValue(double value);
};




void ControlValue::update()
{
    UpdatedValue<double>::update();
    _isActive.update();
}

void ControlValue::setValue(double value)
{
    inputValue(value);
}

void ControlValue::setValue(const bool value)
{
    if (value == true)
        inputValue(1.0);
    else
        inputValue(0.0);
}

void ControlValue::inputValue(double value)
{
    _valueRange.enforceRange(value);

    if (_inactiveRange.checkIfInRange(value))
        _isActive.setValue(false);
    else
        _isActive.setValue(true);

    UpdatedValue<double>::setValue(value);
}

ControlValue::ControlValue()
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude(0.0, 0.2)),
  _valueRange(Bounds(0.0, 1.0)),
  _isActive(false)
{
    setValue(0.0);
}

ControlValue::ControlValue(const double value)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude(0.0, 0.2)),
  _valueRange(Bounds(0.0, 1.0)),
  _isActive(false)
{
    setValue(value);
}

ControlValue::ControlValue(const double value, const Range& inactiveRange)
: UpdatedValue<double>(0.0),
  _inactiveRange(inactiveRange),
  _valueRange(Bounds(0.0, 1.0)),
  _isActive(false)
{
    setValue(value);
}

ControlValue::ControlValue(const double value, const Range& inactiveRange, const Range& valueRange)
: UpdatedValue<double>(0.0),
  _inactiveRange(inactiveRange),
  _valueRange(valueRange),
  _isActive(false)
{
    setValue(value);
}

#endif // CONTROLVALUE_H
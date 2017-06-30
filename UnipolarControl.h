#ifndef UNIPOLARCONTROL_H
#define UNIPOLARCONTROL_H

#include "Activator.h"
#include "Range.h"
#include "Rescale.h"

// This is the basic class for an analog control that goes from 0 to 1.
// it has an inactive range which is basically a deadzone. Within this
// range the Activator portion of it is set inactive, so this can be
// used in places where an Activator is needed.
class UnipolarControl : public Activator
{
public:
    UnipolarControl();
    explicit UnipolarControl(const double value);
    explicit UnipolarControl(const bool value);

    virtual void endCycle();

    virtual void setValue(const double value);
    virtual void setValue(const bool value);

    const double getValue() const              { return _currentValue; }
    const double getPreviousValue() const      { return _previousValue; }

    const Range<double>& getValueRange() const { return _valueRange; }

    const bool valueJustChanged() const       { return _currentValue != _previousValue; }

private:
    double _currentValue;
    double _previousValue;

    Range<double> _inactiveRange;
    Range<double> _valueRange;
};



void UnipolarControl::endCycle()
{
    Activator::endCycle();
    _previousValue = _currentValue;
}

void UnipolarControl::setValue(const double value)
{
    _currentValue = _valueRange.enforceRange(value);

    bool valueIsActive = !_inactiveRange.checkIfInRange(getValue());
    Activator::setState(valueIsActive);
}

void UnipolarControl::setValue(const bool value)
{
    if (value == true)
    {
        _currentValue = _valueRange.enforceRange(1.0);
        Activator::setState(true);
    }

    if (value == false)
    {
        _currentValue = _valueRange.enforceRange(0.0);
        Activator::setState(false);
    }
}

UnipolarControl::UnipolarControl()
: Activator(false),
  _currentValue(0.0),
  _previousValue(0.0),
  _inactiveRange(Range<double>(CenterAndMagnitude<double>(0.0, 0.2))),
  _valueRange(Range<double>(Bounds<double>(0.0, 1.0)))
{
    setValue(0.0);
}

UnipolarControl::UnipolarControl(const double value)
: Activator(false),
  _currentValue(value),
  _previousValue(value),
  _inactiveRange(Range<double>(CenterAndMagnitude<double>(0.0, 0.2))),
  _valueRange(Range<double>(Bounds<double>(0.0, 1.0)))
{
    setValue(value);
    endCycle();
}

UnipolarControl::UnipolarControl(const bool value)
: Activator(false),
  _currentValue(value),
  _previousValue(value),
  _inactiveRange(Range<double>(CenterAndMagnitude<double>(0.0, 0.2))),
  _valueRange(Range<double>(Bounds<double>(0.0, 1.0)))
{
    setValue(value);
    endCycle();
}

#endif // UNIPOLARCONTROL_H
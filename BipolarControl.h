#ifndef BIPOLARCONTROL_H
#define BIPOLARCONTROL_H

#include "Activator.h"
#include "Range.h"
#include "Rescale.h"

class BipolarControl : public Activator
{
public:
    BipolarControl();
    explicit BipolarControl(const double value);

    virtual void endCycle();

    virtual void setValue(const double value);

    const double getValue() const              { return _currentValue; }
    const double getPreviousValue() const      { return _previousValue; }

    const Range<double>& getValueRange() const { return _valueRange; }

    const bool valueJustChanged() const        { return _currentValue != _previousValue; }

    const bool justCrossedInactiveRange() const;

private:
    double _currentValue;
    double _previousValue;

    Range<double> _inactiveRange;
    Range<double> _valueRange;
};



void BipolarControl::endCycle()
{
    Activator::endCycle();
    _previousValue = _currentValue;
}

void BipolarControl::setValue(const double value)
{
    _currentValue = _valueRange.enforceRange(value);

    bool valueIsActive = !_inactiveRange.checkIfInRange(getValue());
    Activator::setState(valueIsActive);
}

const bool BipolarControl::justCrossedInactiveRange() const
{
    bool crossedHighToLow = _previousValue > _inactiveRange.getUpperBound()
                         && _currentValue < _inactiveRange.getLowerBound();

    bool crossedLowToHigh = _previousValue < _inactiveRange.getLowerBound()
                         && _currentValue > _inactiveRange.getUpperBound();

    return crossedHighToLow || crossedLowToHigh;
}

BipolarControl::BipolarControl()
: Activator(false),
  _currentValue(0.0),
  _previousValue(0.0),
  _inactiveRange(Range<double>(CenterAndMagnitude<double>(0.0, 0.2))),
  _valueRange(Range<double>(Bounds<double>(-1.0, 1.0)))
{
    setValue(0.0);
}

BipolarControl::BipolarControl(const double value)
: Activator(false),
  _currentValue(0.0),
  _previousValue(0.0),
  _inactiveRange(Range<double>(CenterAndMagnitude<double>(0.0, 0.2))),
  _valueRange(Range<double>(Bounds<double>(-1.0, 1.0)))
{
    setValue(value);
    endCycle();
}

#endif // BIPOLARCONTROL_H
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

    virtual void update();

    void setValue(const Control& value);
    void setValue(const double value);
    void setValue(const bool value);

    const void setValueRange(const Range<double>& range) { _valueRange = range; }
    const Range<double>& getValueRange() const           { return _valueRange; }

    const bool stateHasChanged() const                   { return _state.hasChanged(); }
    const bool isActive() const                          { return _state.getValue(); }
    const bool justActivated() const                     { return _state.justActivated(); }
    const bool justDeactivated() const                   { return _state.justDeactivated(); }

    const unsigned int getStateTime() const              { return _state.timeSinceChange(); }

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
    _state.setValue(valueIsActive);
}

void Control::setValue(const double value)
{
    UpdatedValue<double>::setValue(_valueRange.enforceRange(value));

    bool valueIsActive = !_inactiveRange.checkIfInRange(getValue());
    _state.setValue(valueIsActive);
}

void Control::setValue(const bool value)
{
    if (value == true)
    {
        UpdatedValue<double>::setValue(_valueRange.enforceRange(1.0));
        _state.setValue(true);
    }

    if (value == false)
    {
        UpdatedValue<double>::setValue(_valueRange.enforceRange(0.0));
        _state.setValue(false);
    }
}

Control::Control()
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(Bounds<double>(0.0, 1.0))
{
    setValue(0.0);
}

Control::Control(const double value)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(Bounds<double>(0.0, 1.0))
{
    setValue(value);
}

Control::Control(const bool value)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(Bounds<double>(0.0, 1.0))
{
    setValue(value);
}

Control::Control(const double value, const Range<double>& valueRange)
: UpdatedValue<double>(0.0),
  _inactiveRange(CenterAndMagnitude<double>(0.0, 0.2)),
  _valueRange(valueRange)
{
    setValue(value);
}

#endif // CONTROL_H
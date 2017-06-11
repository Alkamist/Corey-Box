#ifndef AXISMODIFIER_H
#define AXISMODIFIER_H

#include "ControlValue.h"

// This class is a ControlValue that can modify another control
// value based on what the AxisModifier's value is. It can be
// activated and deactivated with another ControlValue.
class AxisModifier : public ControlValue
{
public:
    AxisModifier(double value,
                 ControlValue& activator);

    void update();

    void setActivator(ControlValue& activator) { _activator = &activator; }
    ControlValue& getActivator()               { return *_activator; }
    virtual void applyModifier(ControlValue& axis);

private:
    ControlValue* _activator;

    void modify(ControlValue& axis);
};

void AxisModifier::update()
{
    ControlValue::update();
}

void AxisModifier::applyModifier(ControlValue& axis)
{
    if (_activator != nullptr)
    {
        if (_activator->isActive())
            modify(axis);
    }
}

void AxisModifier::modify(ControlValue& axis)
{
    double modDecimal = getValue();
    double axisCenter = axis.getDefaultValue();
    double axisValue = axis.getValue();

    if (axisValue < axisCenter)
    {
        axis.setValue(axisCenter - axisCenter * modDecimal);
        return;
    }
    if (axisValue > axisCenter)
    {
        axis.setValue(axisCenter + (1.0 - axisCenter) * modDecimal);
        return;
    }
}

AxisModifier::AxisModifier(double value,
                           ControlValue& activator)
: ControlValue(value),
  _activator(&activator)
{}

#endif // AXISMODIFIER_H
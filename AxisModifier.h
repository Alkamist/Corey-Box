#ifndef AXISMODIFIER_H
#define AXISMODIFIER_H

#include "ControlValue.h"

// This class is a ControlValue that can modify another control
// value based on what the AxisModifier's value is. It can be
// activated and deactivated with another ControlValue.
class AxisModifier : public ControlValue
{
public:
    explicit AxisModifier(const double value);
    explicit AxisModifier(const double value,
                          const ControlValue& activator);

    void setActivator(const ControlValue& activator) { _activator = &activator; }

    const ControlValue& getActivator() const         { return *_activator; }

    void apply(ControlValue& axis) const;
    virtual void modify(ControlValue& axis) const = 0;

private:
    const ControlValue* _activator;
};



void AxisModifier::apply(ControlValue& axis) const
{
    if (_activator != nullptr)
    {
        if (_activator->isActive())
            modify(axis);
    }
}

AxisModifier::AxisModifier(const double value)
: ControlValue(value),
  _activator(nullptr)
{}

AxisModifier::AxisModifier(const double value,
                           const ControlValue& activator)
: ControlValue(value),
  _activator(&activator)
{}

#endif // AXISMODIFIER_H
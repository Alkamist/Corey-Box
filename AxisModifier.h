#ifndef AXISMODIFIER_H
#define AXISMODIFIER_H

#include "Control.h"
#include "ControlSlot.h"

class AxisModifier : public Control
{
public:
    explicit AxisModifier(const double value)
    : Control(value),
      _activator()
    {}

    explicit AxisModifier(const double value, const Control& activator)
    : Control(value),
      _activator(activator)
    {}

    void setActivator(const Control& activator) { _activator.setControl(activator); }

    const Control& getActivator() const         { return _activator.getControl(); }

    void apply(Control& axis) const;

    virtual void modify(Control& axis) const = 0;

private:
    ControlSlot _activator;
};



void AxisModifier::apply(Control& axis) const
{
    if (_activator.isActive())
        modify(axis);
}

#endif // AXISMODIFIER_H
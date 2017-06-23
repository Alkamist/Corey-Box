#ifndef AXISMODIFIER_H
#define AXISMODIFIER_H

#include "Control.h"

class AxisModifier : public Control
{
public:
    explicit AxisModifier(const double value)
    : Control(value)
    {}

    virtual void update()                { _control.update(); }

    void setControls(const bool control) { _control = control; }

    void apply(Control& axis) const;

    virtual void modify(Control& axis) const = 0;

private:
    ControlState _control;
};



void AxisModifier::apply(Control& axis) const
{
    if (_control.getValue())
        modify(axis);
}

#endif // AXISMODIFIER_H
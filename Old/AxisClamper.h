#ifndef AXISCLAMPER_H
#define AXISCLAMPER_H

#include "AxisModifier.h"

class AxisClamper : public AxisModifier
{
public:
    explicit AxisClamper(const double value);
    explicit AxisClamper(const double value,
                         const ControlValue& activator);

    virtual void modify(ControlValue& axis) const;
};



void AxisClamper::modify(ControlValue& axis) const
{
    double lowClamp = axis.getDefaultValue() - getValue();
    double highClamp = axis.getDefaultValue() + getValue();

    if (axis.getValue() < lowClamp)
    {
        axis.setValue(lowClamp);
        return;
    }
    if (axis.getValue() > highClamp)
    {
        axis.setValue(highClamp);
        return;
    }
}

AxisClamper::AxisClamper(const double value)
: AxisModifier(value)
{}

AxisClamper::AxisClamper(const double value,
                         const ControlValue& activator)
: AxisModifier(value, activator)
{}

#endif // AXISCLAMPER_H
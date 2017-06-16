#ifndef AXISCLAMPER_H
#define AXISCLAMPER_H

#include "AxisModifier.h"

class AxisClamper : public AxisModifier
{
public:
    explicit AxisClamper(const double value);
    explicit AxisClamper(const double value, const ControlValue& activator);

    virtual void modify(AnalogAxis& axis) const;
};



void AxisClamper::modify(AnalogAxis& axis) const
{
    double modDecimal = getValue();
    double axisCenter = axis.getDefaultValue();
    double axisValue = axis.getValue();

    double lowClamp = axisCenter - axisCenter * modDecimal;
    double highClamp = axisCenter + (1.0 - axisCenter) * modDecimal;

    if (axisValue < lowClamp)
    {
        axis.setValue(lowClamp);
        return;
    }

    if (axisValue > highClamp)
    {
        axis.setValue(highClamp);
        return;
    }
}

AxisClamper::AxisClamper(const double value)
: AxisModifier(value)
{}

AxisClamper::AxisClamper(const double value, const ControlValue& activator)
: AxisModifier(value, activator)
{}

#endif // AXISCLAMPER_H
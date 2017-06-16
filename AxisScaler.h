#ifndef AXISSCALER_H
#define AXISSCALER_H

#include "AxisModifier.h"

class AxisScaler : public AxisModifier
{
public:
    explicit AxisScaler(const double value);
    explicit AxisScaler(const double value, const ControlValue& activator);

    virtual void modify(AnalogAxis& axis) const;
};



void AxisScaler::modify(AnalogAxis& axis) const
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

AxisScaler::AxisScaler(const double value)
: AxisModifier(value)
{}

AxisScaler::AxisScaler(const double value, const ControlValue& activator)
: AxisModifier(value, activator)
{}

#endif // AXISSCALER_H
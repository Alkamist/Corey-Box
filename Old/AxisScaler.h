#ifndef AXISSCALER_H
#define AXISSCALER_H

#include "AxisModifier.h"
#include "ValueScaler.h"

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

    Range oldRange;
    Range newRange;

    if (axisValue < axisCenter)
    {
        double newBound = axisCenter - axisCenter * modDecimal;

        oldRange.setLowerBound(0.0);
        oldRange.setUpperBound(axisCenter);

        newRange.setLowerBound(newBound);
        newRange.setUpperBound(axisCenter);
    }
    if (axisValue > axisCenter)
    {
        double newBound = axisCenter + (1.0 - axisCenter) * modDecimal;

        oldRange.setLowerBound(axisCenter);
        oldRange.setUpperBound(1.0);

        newRange.setLowerBound(axisCenter);
        newRange.setUpperBound(newBound);
    }

    if (axisValue != axisCenter)
    {
        ValueScaler valueScaler(oldRange, newRange);

        axis.setValue(valueScaler.rescaleValue(axisValue));
    }
}

AxisScaler::AxisScaler(const double value)
: AxisModifier(value)
{}

AxisScaler::AxisScaler(const double value, const ControlValue& activator)
: AxisModifier(value, activator)
{}

#endif // AXISSCALER_H
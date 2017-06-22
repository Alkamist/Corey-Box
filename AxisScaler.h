#ifndef AXISSCALER_H
#define AXISSCALER_H

#include "AxisModifier.h"

class AxisScaler : public AxisModifier
{
public:
    explicit AxisScaler(const double value)
    : AxisModifier(value)
    {}

    explicit AxisScaler(const double value, const Control& activator)
    : AxisModifier(value, activator)
    {}

    void modify(Control& axis) const
    {
        Control newControl(0.0, Range<double>(Bounds<double>(-1.0, 1.0)));

        newControl.setValue(axis);

        newControl.setValue(newControl.getValue() * getValue());

        axis.setValue(newControl);
    }
};

#endif // AXISSCALER_H
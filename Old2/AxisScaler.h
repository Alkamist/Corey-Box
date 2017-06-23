#ifndef AXISSCALER_H
#define AXISSCALER_H

#include "AxisModifier.h"

class AxisScaler : public AxisModifier
{
public:
    explicit AxisScaler(const double value)
    : AxisModifier(value)
    {}

    void modify(Control& axis) const
    {
        Control newControl(0.0, Range<double>(Bounds<double>(-1.0, 1.0)));

        newControl = axis;

        newControl = newControl.getValue() * getValue();

        axis = newControl;
    }
};

#endif // AXISSCALER_H
#ifndef AXISCLAMPER_H
#define AXISCLAMPER_H

#include "AxisModifier.h"

class AxisClamper : public AxisModifier
{
public:
    explicit AxisClamper(const double value)
    : AxisModifier(value)
    {}

    explicit AxisClamper(const double value, const Control& activator)
    : AxisModifier(value, activator)
    {}

    void modify(Control& axis) const
    {
        double modDecimal = getValue();
        double axisCenter = 0.0;
        double axisValue = axis.getValue();

        double lowClamp = -modDecimal;
        double highClamp = modDecimal;

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
};

#endif // AXISCLAMPER_H
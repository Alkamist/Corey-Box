#ifndef AXISCLAMPER_H
#define AXISCLAMPER_H

#include "AxisModifier.h"

class AxisClamper : public AxisModifier
{
public:
    explicit AxisClamper(const double value)
    : AxisModifier(value)
    {}

    void modify(Control& axis) const
    {
        double modDecimal = getValue();
        double axisValue = axis.getValue();

        double lowClamp = -modDecimal;
        double highClamp = modDecimal;

        if (axisValue < lowClamp)
        {
            axis = lowClamp;
            return;
        }

        if (axisValue > highClamp)
        {
            axis = highClamp;
            return;
        }
    }
};

#endif // AXISCLAMPER_H
#ifndef ANALOGAXIS_H
#define ANALOGAXIS_H

#include "ControlValue.h"

// This class is simply a ControlValue that has been initialized to
// the values you would expect from an analog axis.
class AnalogAxis : public ControlValue
{
public:
    AnalogAxis();
    explicit AnalogAxis(const double value);
    explicit AnalogAxis(const double value, const Range& inactiveRange);
};



AnalogAxis::AnalogAxis()
: ControlValue(0.5, CenterAndMagnitude(0.5, 0.2))
{}

AnalogAxis::AnalogAxis(const double value)
: ControlValue(value, CenterAndMagnitude(0.5, 0.2))
{}

AnalogAxis::AnalogAxis(const double value, const Range& inactiveRange)
: ControlValue(value, inactiveRange)
{}

#endif // ANALOGAXIS_H
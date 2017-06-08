#ifndef ANALOGAXIS_H
#define ANALOGAXIS_H

#include "AnalogButton.h"

// This class is an arbitrary implementation of an
// analog axis.
class AnalogAxis
{
public:
    AnalogAxis() {}

    void update();

    bool hasChanged();

    void setValue(double value);
    void setDeadZone(double deadZone);

    double getValue();

    inline AnalogButton& getNegativeSide() { return _negativeSide; }
    inline AnalogButton& getPositiveSide() { return _positiveSide; }

private:
    AnalogButton _negativeSide;
    AnalogButton _positiveSide;
};



void AnalogAxis::update()
{
    _negativeSide.update();
    _positiveSide.update();
}

bool AnalogAxis::hasChanged()
{
    return _negativeSide.hasChanged()
        || _positiveSide.hasChanged();
}

void AnalogAxis::setValue(double value)
{
    if (value > 0.0)
    {
        _negativeSide.setValue(0.0);
        _positiveSide.setValue(value);
    }
    else if (value < 0.0)
    {
        _negativeSide.setValue(value);
        _positiveSide.setValue(0.0);
    }
    else
    {
        _negativeSide.setValue(0.0);
        _positiveSide.setValue(0.0);
    }
}

void AnalogAxis::setDeadZone(double deadZone)
{
    _negativeSide.setDeadZone(deadZone);
    _positiveSide.setDeadZone(deadZone);
}

double AnalogAxis::getValue()
{
    if (_positiveSide.getValue() > 0.0)
        return _positiveSide.getValue();

    else if (_positiveSide.getValue() > 0.0)
        return -_negativeSide.getValue();
}

#endif // ANALOGAXIS_H
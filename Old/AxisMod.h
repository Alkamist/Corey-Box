#ifndef AXISMOD_H
#define AXISMOD_H

#include "DigitalButton.h"

class AxisMod
{
public:
    AxisMod::AxisMod(DigitalButton& modButton)
     : _modButton(&modButton),
       _value(0.0),
       _modValue(1.0)
    {}

    void update();

    inline void setModValue(double value) { _modValue = value; }
    inline void setValue(double value)    { _value = value; }
    inline double getValue()              { return _value; }

private:
    DigitalButton* _modButton;

    double _value;
    double _modValue;

    double calculateModdedValue(double axisValue);
};

void AxisMod::update()
{
    if (_modButton.pressed())
    {
        _value = calculateModdedValue(_value);
    }
}

double AxisMod::calculateModdedValue(double axisValue)
{
    if (axisValue < 0.50)
    {
        axisValue = 0.50 - 0.50 * _modValue;
        return axisValue;
    }
    if (axisValue > 0.50)
    {
        axisValue = 0.50 + 0.50 * _modValue;
        return axisValue;
    }
}

#endif // AXISMOD_H
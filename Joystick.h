#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"
#include "ScaleBipolar.h"

class Joystick
{
public:
    Joystick()
    : xValue(128),
      yValue(128),
      _magnitude(0)
    {}

    Control xValue;
    Control yValue;

    virtual void process()
    {
        if (xValue.justChanged() || yValue.justChanged())
        {
            _magnitude = calculateMagnitude(xValue, yValue);

            limitToMagnitude(45);
        }
    }

    virtual void endCycle()
    {
        xValue.endCycle();
        yValue.endCycle();
    }

    void limitToMagnitude(const uint8_t value)
    {
        if (_magnitude > value)
            scaleToMagnitude(value);
    }

    void scaleToMagnitude(const uint8_t value)
    {
        if (_magnitude != 0)
        {
            float scaleFactor = value / static_cast<float>(_magnitude);

            uint8_t xScale = xValue.getBipolarMagnitude() * scaleFactor;
            uint8_t yScale = yValue.getBipolarMagnitude() * scaleFactor;

            xValue = scaleBipolar(xValue, xScale);
            yValue = scaleBipolar(yValue, yScale);

            _magnitude = value;
        }
    }

    const uint8_t getMagnitude() const { return _magnitude; }

private:
    uint8_t _magnitude;

    const uint8_t calculateMagnitude(const uint8_t x, const uint8_t y) const { return sqrt(square(x - 128) + square(y - 128)); }
};

#endif // JOYSTICK_H

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"
#include "ScaleBipolar.h"

// This is a basic joystick with an x and y value. It will automatically software-gate
// itself to a magnitude of 127, to simulate a circular gate like Melee does internally.
class Joystick
{
public:
    Joystick()
    : xValue(128),
      yValue(128),
      _magnitude(0),
      _range(127)
    {}

    Control xValue;
    Control yValue;

    virtual void process()
    {
        if (xValue.justChanged() || yValue.justChanged())
        {
            _magnitude = calculateMagnitude(xValue.getBipolarMagnitude(), yValue.getBipolarMagnitude());

            limitToMagnitude(_range);
        }
    }

    virtual void endCycle()
    {
        xValue.endCycle();
        yValue.endCycle();
    }

    void limitToMagnitude(const uint8_t newMagnitude)
    {
        if (_magnitude > newMagnitude)
            scaleToMagnitude(newMagnitude);
    }

    void scaleToMagnitude(const uint8_t newMagnitude)
    {
        if (_magnitude != 0)
        {
            uint8_t scaleFactor = (127 * newMagnitude) / _magnitude;

            xValue = scaleBipolar(xValue, scaleFactor);
            yValue = scaleBipolar(yValue, scaleFactor);

            _magnitude = newMagnitude;
        }
    }

    const uint8_t getMagnitude() const         { return _magnitude; }

    virtual void setRange(const uint8_t value) { _range = value; }

private:
    uint8_t _magnitude;
    uint8_t _range;

    const uint8_t calculateMagnitude(const uint8_t x, const uint8_t y) const { return sqrt(sq(x) + sq(y)); }
};

#endif // JOYSTICK_H

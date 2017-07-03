#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"

// This class represents a joystick that can tell whether or not it
// is active based on its distance away from the origin.
class Joystick : public Activator
{
public:
    Joystick()
    : Activator(),
      _inactiveRadius(0.2),
      _distance(0.0)
    {
        _xValue.makeBipolar();
        _yValue.makeBipolar();
    }

    virtual void process()
    {
        if (_xValue.valueJustChanged() || _yValue.valueJustChanged())
        {
            _distance = calculateDistance(_xValue.getValue(), _yValue.getValue());
            setState(_distance > _inactiveRadius);
        }
    }

    virtual void endCycle()
    {
        Activator::endCycle();
        _xValue.endCycle();
        _yValue.endCycle();
    }

    virtual void setXValue(const Control& value) { _xValue = value; }
    virtual void setYValue(const Control& value) { _yValue = value; }
    virtual void setXValue(const float value)    { _xValue = value; }
    virtual void setYValue(const float value)    { _yValue = value; }

    const Control& getXControl() const           { return _xValue; }
    const Control& getYControl() const           { return _yValue; }
    const float getXValue() const                { return _xValue.getValue(); }
    const float getYValue() const                { return _yValue.getValue(); }

    const float getXMagnitude() const            { return abs(_xValue.getValue()); }
    const float getYMagnitude() const            { return abs(_yValue.getValue()); }
    const float getMagnitude() const             { return calculateDistance(_xValue.getValue(), _yValue.getValue()); }

private:
    Control _xValue;
    Control _yValue;

    float _inactiveRadius;
    float _distance;

    const float calculateDistance(const float x, const float y) const { return sqrt(square(x) + square(y)); }
};

#endif // JOYSTICK_H

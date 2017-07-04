#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"

// This class represents a joystick that can tell whether or not it
// is active based on its distance away from the origin. The x and y
// values are also normalized so their vector magnitude does not
// exceed 1.0.
class Joystick : public Activator
{
public:
    Joystick()
    : Activator(),
      _inactiveRadius(0.2),
      _magnitude(0.0)
    {
        _xValue.makeBipolar();
        _yValue.makeBipolar();

        _deadZone.setLowerBound(0.0);
        _deadZone.setUpperBound(0.2600);

        _tiltZone.setLowerBound(0.2875);
        _tiltZone.setUpperBound(0.7875);

        _smashZone.setLowerBound(0.8000);
        _smashZone.setUpperBound(1.0);

        _crouchZone.setLowerBound(-1.0);
        _crouchZone.setUpperBound(-0.6125);
    }

    virtual void process()
    {
        if (_xValue.valueJustChanged() || _yValue.valueJustChanged())
        {
            _magnitude = calculateMagnitude(_xValue.getValue(), _yValue.getValue());

            limitToMagnitude(1.0);

            setState(_magnitude > _inactiveRadius);
        }
    }

    virtual void endCycle()
    {
        Activator::endCycle();
        _xValue.endCycle();
        _yValue.endCycle();
    }

    void limitToMagnitude(const float value)
    {
        if (_magnitude > value)
            scaleToMagnitude(value);
    }

    void scaleToMagnitude(const float value)
    {
        if (_magnitude != 0.0)
        {
            _xValue *= value / _magnitude;
            _yValue *= value / _magnitude;

            _magnitude = value;
        }
    }

    void setXValue(const Control& value) { _xValue = value; }
    void setYValue(const Control& value) { _yValue = value; }
    void setXValue(const float value)    { _xValue = value; }
    void setYValue(const float value)    { _yValue = value; }

    const Control& getXControl() const   { return _xValue; }
    const Control& getYControl() const   { return _yValue; }
    const float getXValue() const        { return _xValue.getValue(); }
    const float getYValue() const        { return _yValue.getValue(); }

    const float getXMagnitude() const    { return abs(_xValue.getValue()); }
    const float getYMagnitude() const    { return abs(_yValue.getValue()); }
    const float getMagnitude() const     { return _magnitude; }

    const Range<float>& getDeadZone() const   { return _deadZone; }
    const Range<float>& getTiltZone() const   { return _tiltZone; }
    const Range<float>& getSmashZone() const  { return _smashZone; }
    const Range<float>& getCrouchZone() const { return _crouchZone; }

private:
    Control _xValue;
    Control _yValue;

    Range<float> _deadZone;
    Range<float> _tiltZone;
    Range<float> _smashZone;
    Range<float> _crouchZone;

    float _inactiveRadius;
    float _magnitude;

    const float calculateMagnitude(const float x, const float y) const { return sqrt(square(x) + square(y)); }
};

#endif // JOYSTICK_H

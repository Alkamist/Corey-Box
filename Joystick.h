#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "BipolarControl.h"

class Joystick : public Activator
{
public:
    Joystick()
    : Activator(),
      _inactiveRadius(0.2),
      _distance(0.0),
      _range(0.6250)
    {}

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

    void setRange(const double range) { _range = range; }

    virtual void setXValue(const double value) { _xValue.setValue(value * _range); }
    virtual void setYValue(const double value) { _yValue.setValue(value * _range); }

    const BipolarControl& getXControl() const { return _xValue; }
    const BipolarControl& getYControl() const { return _yValue; }

    const double calculateDistance(const double x, const double y) { return sqrt(square(x) + square(y)); }

private:
    BipolarControl _xValue;
    BipolarControl _yValue;

    double _inactiveRadius;
    double _distance;
    double _range;
};

#endif // JOYSTICK_H

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "BipolarControl.h"

class Joystick : public Activator
{
public:
    Joystick(const BipolarControl& xControl, const BipolarControl& yControl)
    : Activator(),
      _xControl(xControl),
      _yControl(yControl),
      _inactiveRadius(0.2),
      _distance(0.0),
      _range(0.6250)
    {}

    virtual void process()
    {
        if (_xControl.valueJustChanged())
            _xValue.setValue(_xControl.getValue() * _range);
        if (_yControl.valueJustChanged())
            _yValue.setValue(_yControl.getValue() * _range);

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

    const BipolarControl& getXControl() const { return _xValue; }
    const BipolarControl& getYControl() const { return _yValue; }

    const double calculateDistance(const double x, const double y) { return sqrt(square(x) + square(y)); }

private:
    const BipolarControl& _xControl;
    const BipolarControl& _yControl;

    BipolarControl _xValue;
    BipolarControl _yValue;

    double _inactiveRadius;
    double _distance;
    double _range;
};

#endif // JOYSTICK_H

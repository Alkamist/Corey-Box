#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"

class Joystick
{
public:
    explicit Joystick()
    : _xValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _yValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _inactiveRadius(0.2),
      _distance(0.0)
    {}

    virtual void update()
    {
        _xValue.update();
        _yValue.update();
        _state.update();
    }

    virtual void setControls(const Control& xValue, const Control& yValue)
    {
        _xValue = xValue;
        _yValue = yValue;

        if (_xValue.hasChanged() || _yValue.hasChanged())
            _distance = calculateDistance(_xValue.getValue(), _yValue.getValue());

        _state.setValue(_distance > _inactiveRadius);
    }

    const Control& getXControl() const { return _xValue; }
    const Control& getYControl() const { return _yValue; }

    const bool hasMoved() const        { return _xValue.hasChanged() || _yValue.hasChanged(); }
    const bool isActive() const        { return _state.getValue(); }
    const bool justActivated() const   { return _state.justActivated(); }
    const bool justDeactivated() const { return _state.justDeactivated(); }

    const double calculateDistance(const double x,
                                   const double y)
    {
        return sqrt(square(x) + square(y));
    }

private:
    Control _xValue;
    Control _yValue;

    ControlState _state;

    double _inactiveRadius;
    double _distance;
};

#endif // JOYSTICK_H
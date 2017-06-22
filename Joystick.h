#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"
#include "ControlSlot.h"

class Joystick
{
public:
    explicit Joystick(const Control& xControl, const Control& yControl)
    : _xValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _yValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _xControl(xControl),
      _yControl(yControl),
      _inactiveRadius(0.2),
      _maxRadius(1.0),
      _distance(0.0)
    {}

    virtual void update();

    const Control& getXValue() const   { return _xValue; }
    const Control& getYValue() const   { return _yValue; }

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

    ControlSlot _xControl;
    ControlSlot _yControl;

    double _inactiveRadius;
    double _maxRadius;

    double _distance;

    ControlState _state;
};



void Joystick::update()
{
    _xValue.update();
    _yValue.update();
    _state.update();

    if (_xControl.hasChanged() || _yControl.hasChanged())
    {
        _xValue.setValue(_xControl.getControl());
        _yValue.setValue(_yControl.getControl());
        _distance = calculateDistance(_xValue.getValue(), _yValue.getValue());
    }

    _state.setValue(_distance > _inactiveRadius);
}

#endif // JOYSTICK_H
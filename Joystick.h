#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"
#include "Signum.h"

class Joystick
{
public:
    Joystick()
    : _xValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _yValue(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _inactiveRadius(0.2),
      _maxRadius(1.0)
    {}

    virtual void update();

    void setXValue(const Control& value);
    void setYValue(const Control& value);

    const Control& getXValue() const            { return _xValue; }
    const Control& getYValue() const            { return _yValue; }

    void setCoordinates(const Control& x, const Control& y);

    void setInactiveRadius(const double radius) { _inactiveRadius = radius; }
    void setMaxRadius(const double radius)      { _maxRadius = radius; }

    const bool hasMoved() const                 { return _xValue.hasChanged() || _yValue.hasChanged(); }
    const bool isActive() const                 { return _state.getValue(); }
    const bool justActivated() const            { return _state.justActivated(); }
    const bool justDeactivated() const          { return _state.justDeactivated(); }

    const double getDistanceFromOrigin() const  { return sqrt(square(_xValue.getValue()) + square(_yValue.getValue()));; }

private:
    Control _xValue;
    Control _yValue;

    double _inactiveRadius;
    double _maxRadius;

    ControlState _state;

    void calculateOtherAxis(Control& axisToSet, Control& otherAxis);
};



void Joystick::update()
{
    _xValue.update();
    _yValue.update();
    _state.update();

    _state.setValue(_distance > _inactiveRadius);
}

void Joystick::setXValue(const Control& value)
{
    _xValue.setValue(value);

    calculateOtherAxis(_yValue, _xValue);
}

void Joystick::setYValue(const Control& value)
{
    _yValue.setValue(value);

    calculateOtherAxis(_xValue, _yValue);
}

void Joystick::setCoordinates(const Control& x, const Control& y)
{
    _xValue.setValue(x);
    _yValue.setValue(y);
}

void Joystick::calculateOtherAxis(Control& axisToSet, Control& otherAxis)
{
    if (getDistanceFromOrigin() > _maxRadius)
        axisToSet.setValue(signum(axisToSet.getValue()) * sqrt(1.0 - square(otherAxis.getValue())));
}

#endif // JOYSTICK_H
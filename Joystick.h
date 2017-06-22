#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

#include "Control.h"
#include "ControlSlot.h"
#include "Signum.h"

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

private:
    Control _xValue;
    Control _yValue;

    ControlSlot _xControl;
    ControlSlot _yControl;

    double _inactiveRadius;
    double _maxRadius;

    double _distance;

    ControlState _state;

    void calculateOtherAxis(Control& axisToSet, Control& otherAxis);
    void calculateDistance() { _distance = sqrt(square(_xValue.getValue()) + square(_yValue.getValue())); }
};



void Joystick::update()
{
    _xValue.update();
    _yValue.update();
    _state.update();

    if (_xControl.hasChanged())
    {
        _xValue.setValue(_xControl);
        calculateDistance();
    }

    if (_yControl.hasChanged())
    {
        _xValue.setValue(_yControl);
        calculateDistance()
    }

    _state.setValue(_distance > _inactiveRadius);
}

void Joystick::calculateOtherAxis(Control& axisToSet, Control& otherAxis)
{
    if (_distance > _maxRadius)
        axisToSet.setValue(signum(axisToSet.getValue()) * sqrt(1.0 - square(otherAxis.getValue())));
}

#endif // JOYSTICK_H
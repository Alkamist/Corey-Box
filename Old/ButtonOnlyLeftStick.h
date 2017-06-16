#ifndef BUTTONONLYLEFTSTICK_H
#define BUTTONONLYLEFTSTICK_H

#include "DoubleModAxis.h"
#include "Timer.h"

class ButtonOnlyLeftStick
{
public:
    explicit ButtonOnlyLeftStick(const ControlValue& xAxisLow,
                                 const ControlValue& xAxisHigh,
                                 const ControlValue& xMod1,
                                 const ControlValue& xMod2,
                                 const ControlValue& yAxisLow,
                                 const ControlValue& yAxisHigh,
                                 const ControlValue& yMod1,
                                 const ControlValue& yMod2,
                                 const ControlValue& tilt,
                                 const ControlValue& tiltTempDisable,
                                 const ControlValue& shieldDrop);

    void update();

    ControlValue getXValue();
    ControlValue getYValue();

private:
    DoubleModAxis _xAxis;
    DoubleModAxis _yAxis;

    Timer _shieldDropTimer;

    const ControlValue* _shieldDrop;
};



void ButtonOnlyLeftStick::update()
{
    _xAxis.update();
    _yAxis.update();

    if (_shieldDrop->justActivated())
        _shieldDropTimer.reset();
}

ControlValue ButtonOnlyLeftStick::getXValue()
{
    if (!_shieldDropTimer.targetTimeReached())
        _xAxis.setValue(0.5);

    return _xAxis;
}

ControlValue ButtonOnlyLeftStick::getYValue()
{
    if (!_shieldDropTimer.targetTimeReached())
        _yAxis.setValue(0.4);

    return _yAxis;
}

ButtonOnlyLeftStick::ButtonOnlyLeftStick(const ControlValue& xAxisLow,
                                         const ControlValue& xAxisHigh,
                                         const ControlValue& xMod1,
                                         const ControlValue& xMod2,
                                         const ControlValue& yAxisLow,
                                         const ControlValue& yAxisHigh,
                                         const ControlValue& yMod1,
                                         const ControlValue& yMod2,
                                         const ControlValue& tilt,
                                         const ControlValue& tiltTempDisable,
                                         const ControlValue& shieldDrop)
: _xAxis(xAxisLow, xAxisHigh, xMod1, xMod2, tilt, tiltTempDisable),
  _yAxis(yAxisLow, yAxisHigh, yMod1, yMod2, tilt, tiltTempDisable),
  _shieldDrop(&shieldDrop),
  _shieldDropTimer(24)
{}

#endif // BUTTONONLYLEFTSTICK_H
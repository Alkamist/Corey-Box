#ifndef BUTTONONLYCSTICK_H
#define BUTTONONLYCSTICK_H

#include "TwoButtonControl.h"
#include "AxisScaler.h"

class ButtonOnlyCStick
{
public:
    explicit ButtonOnlyCStick(const ControlValue& xAxisLow, const ControlValue& xAxisHigh,
                              const ControlValue& yAxisLow, const ControlValue& yAxisHigh,
                              const ControlValue& lsYLow, const ControlValue& lsYHigh,
                              const ControlValue& tilt);

    void update();

    ControlValue getXValue();
    ControlValue getYValue();

private:
    const ControlValue* _lsYLow;
    const ControlValue* _lsYHigh;
    const ControlValue* _yAxisLow;
    const ControlValue* _yAxisHigh;
    const ControlValue* _tilt;

    TwoButtonControl _xAxis;
    TwoButtonControl _yAxis;

    AxisScaler _yAxisSkew;

    ControlValue _yAxisSkewActivator;
    ControlValue _yAxisLowActivator;
    ControlValue _yAxisHighActivator;

    void resolveActivators();
};



void ButtonOnlyCStick::update()
{
    _xAxis.update();
    _yAxis.update();
    _yAxisSkewActivator.update();
    _yAxisLowActivator.update();
    _yAxisHighActivator.update();

    _yAxisSkew.apply(_yAxis);

    resolveActivators();
}

void ButtonOnlyCStick::resolveActivators()
{
    bool skewDown = _tilt->isActive() && _lsYLow->isActive() && _xAxis.isActive();
    bool skewUp   = _tilt->isActive() && _lsYHigh->isActive() && _xAxis.isActive();

    bool skewShouldHappen = (skewDown || skewUp)
                        && !(_yAxisLow->isActive() || _yAxisHigh->isActive());

    _yAxisLowActivator.setValue(skewDown || _yAxisLow->isActive());
    _yAxisHighActivator.setValue(skewUp || _yAxisHigh->isActive());

    _yAxisSkewActivator.setValue(skewShouldHappen);
}

ControlValue ButtonOnlyCStick::getXValue()
{
    return _xAxis;
}

ControlValue ButtonOnlyCStick::getYValue()
{
    return _yAxis;
}

ButtonOnlyCStick::ButtonOnlyCStick(const ControlValue& xAxisLow, const ControlValue& xAxisHigh,
                                   const ControlValue& yAxisLow, const ControlValue& yAxisHigh,
                                   const ControlValue& lsYLow, const ControlValue& lsYHigh,
                                   const ControlValue& tilt)
: _lsYLow(&lsYLow),
  _lsYHigh(&lsYHigh),
  _yAxisLow(&yAxisLow),
  _yAxisHigh(&yAxisHigh),
  _tilt(&tilt),
  _xAxis(xAxisLow, xAxisHigh),
  _yAxis(_yAxisLowActivator, _yAxisHighActivator),
  _yAxisSkew(0.5, _yAxisSkewActivator)
{}

#endif // BUTTONONLYCSTICK_H
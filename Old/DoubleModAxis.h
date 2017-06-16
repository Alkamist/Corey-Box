#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"
#include "AxisScaler.h"
#include "AxisTilter.h"

class DoubleModAxis : public TwoButtonControl
{
public:
    explicit DoubleModAxis(const ControlValue& lowControl,
                           const ControlValue& highControl,
                           const ControlValue& mod1Control,
                           const ControlValue& mod2Control,
                           const ControlValue& tiltControl,
                           const ControlValue& tiltTempDisableControl);

    void update();

private:
    const ControlValue* _mod1Control;
    const ControlValue* _mod2Control;

    ControlValue _mod1Activator;
    ControlValue _mod2Activator;
    ControlValue _mod3Activator;
    ControlValue _tiltTimerResetActivator;

    AxisScaler _mod1;
    AxisScaler _mod2;
    AxisScaler _mod3;

    AxisTilter _axisTilter;

    void resolveActivators();
};



void DoubleModAxis::update()
{
    TwoButtonControl::update();

    _mod1Activator.update();
    _mod2Activator.update();
    _mod3Activator.update();
    _tiltTimerResetActivator.update();
    _mod1.update();
    _mod2.update();
    _mod3.update();
    _axisTilter.update();

    _mod1.apply(*this);
    _mod2.apply(*this);
    _mod3.apply(*this);
    _axisTilter.apply(*this);

    resolveActivators();
}

void DoubleModAxis::resolveActivators()
{
    _mod1Activator = _mod1Control->isActive()
                  && !_mod2Control->isActive();

    _mod2Activator = !_mod1Control->isActive()
                  && _mod2Control->isActive();

    _mod3Activator = _mod1Control->isActive()
                  && _mod2Control->isActive();

    _tiltTimerResetActivator = !isActive();
}

DoubleModAxis::DoubleModAxis(const ControlValue& lowControl,
                             const ControlValue& highControl,
                             const ControlValue& mod1Control,
                             const ControlValue& mod2Control,
                             const ControlValue& tiltControl,
                             const ControlValue& tiltTempDisableControl)
: TwoButtonControl(lowControl, highControl),
  _mod1Control(&mod1Control),
  _mod2Control(&mod2Control),
  _mod1(0.30, _mod1Activator),
  _mod2(0.35, _mod2Activator),
  _mod3(0.70, _mod3Activator),
  _axisTilter(0.3, 108, 50, tiltControl, _tiltTimerResetActivator, tiltTempDisableControl)
{}

#endif // DOUBLEMODAXIS_H
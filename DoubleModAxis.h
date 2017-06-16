#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"
#include "AnalogAxis.h"
#include "AxisScaler.h"
#include "AxisClamper.h"
#include "TimedActivator.h"

class DoubleModAxis : public AnalogAxis
{
public:
    explicit DoubleModAxis(const ControlValue& lowControl, const ControlValue& highControl,
                           const ControlValue& mod1Control, const ControlValue& mod2Control,
                           const ControlValue& tiltControl, const ControlValue& tiltTempDisableControl);

    virtual void update();

private:
    TwoButtonControl _twoButtonControl;

    const ControlValue* _mod1Control;
    const ControlValue* _mod2Control;

    ControlValue _mod1Activator;
    ControlValue _mod2Activator;
    ControlValue _mod3Activator;
    TimedActivator _tiltActivator;

    AxisScaler _mod1;
    AxisScaler _mod2;
    AxisScaler _mod3;
    AxisClamper _tiltMod;

    void resolveActivators();
};



void DoubleModAxis::update()
{
    AnalogAxis::update();

    _twoButtonControl.update();

    _mod1Activator.update();
    _mod2Activator.update();
    _mod3Activator.update();
    _tiltActivator.update();
    _mod1.update();
    _mod2.update();
    _mod3.update();
    _tiltMod.update();

    setValue(_twoButtonControl.getValue());

    _mod1.apply(*this);
    _mod2.apply(*this);
    _mod3.apply(*this);
    _tiltMod.apply(*this);

    resolveActivators();
}

void DoubleModAxis::resolveActivators()
{
    _mod1Activator.setValue(_mod1Control->isActive()
                        && !_mod2Control->isActive());

    _mod2Activator.setValue(!_mod1Control->isActive()
                          && _mod2Control->isActive());

    _mod3Activator.setValue(_mod1Control->isActive()
                         && _mod2Control->isActive());
}

DoubleModAxis::DoubleModAxis(const ControlValue& lowControl, const ControlValue& highControl,
                             const ControlValue& mod1Control, const ControlValue& mod2Control,
                             const ControlValue& tiltControl, const ControlValue& tiltTempDisableControl)
: AnalogAxis(),
  _twoButtonControl(lowControl, highControl),
  _mod1Control(&mod1Control),
  _mod2Control(&mod2Control),
  _mod1(0.40, _mod1Activator),
  _mod2(0.60, _mod2Activator),
  _mod3(0.80, _mod3Activator),
  _tiltActivator(200, tiltControl, _twoButtonControl),
  _tiltMod(0.10, _tiltActivator)
{}

#endif // DOUBLEMODAXIS_H
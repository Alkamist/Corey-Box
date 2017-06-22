#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"
#include "Control.h"
#include "ControlSlot.h"
#include "AxisScaler.h"

class DoubleModAxis : public Control
{
public:
    explicit DoubleModAxis(const Control& lowControl, const Control& highControl,
                           const Control& mod1Control, const Control& mod2Control);

    virtual void update();

private:
    TwoButtonControl _twoButtonControl;

    ControlSlot _mod1Control;
    ControlSlot _mod2Control;

    Control _mod1Activator;
    Control _mod2Activator;
    Control _mod3Activator;

    AxisScaler _mod1;
    AxisScaler _mod2;
    AxisScaler _mod3;

    void resolveActivators();
};



void DoubleModAxis::update()
{
    Control::update();

    _twoButtonControl.update();

    _mod1Activator.update();
    _mod2Activator.update();
    _mod3Activator.update();

    _mod1.update();
    _mod2.update();
    _mod3.update();

    _twoButtonControl.update();

    setValue(_twoButtonControl.getValue());

    _mod1.apply(*this);
    _mod2.apply(*this);
    _mod3.apply(*this);

    resolveActivators();
}

void DoubleModAxis::resolveActivators()
{
    _mod1Activator.setValue(_mod1Control.isActive()
                        && !_mod2Control.isActive());

    _mod2Activator.setValue(!_mod1Control.isActive()
                          && _mod2Control.isActive());

    _mod3Activator.setValue(_mod1Control.isActive()
                         && _mod2Control.isActive());
}

DoubleModAxis::DoubleModAxis(const Control& lowControl, const Control& highControl,
                             const Control& mod1Control, const Control& mod2Control)
: Control(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
  _twoButtonControl(lowControl, highControl),
  _mod1Control(mod1Control),
  _mod2Control(mod2Control),
  _mod1(0.3025, _mod1Activator),
  _mod2(0.53, _mod2Activator),
  _mod3(0.63, _mod3Activator)
{}

#endif // DOUBLEMODAXIS_H

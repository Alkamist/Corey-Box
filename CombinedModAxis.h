#ifndef COMBINEDMODAXIS_H
#define COMBINEDMODAXIS_H

#include "TwoButtonControl.h"
#include "ModdedAxis.h"
#include "TiltedAxisModifier.h"
#include "ControlValue.h"

class CombinedModAxis : public ModdedAxis
{
public:
    CombinedModAxis(ControlValue& lowControl,
                    ControlValue& highControl,
                    ControlValue& mod1Control,
                    ControlValue& mod2Control,
                    ControlValue& tiltControl,
                    ControlValue& tiltTempDisableControl);

    void update();

private:
    TwoButtonControl _twoButtonController;

    ControlValue* _mod1Control;
    ControlValue* _mod2Control;

    ControlValue _mod1Activator;
    ControlValue _mod2Activator;
    ControlValue _mod3Activator;
    ControlValue _modFullActivator;
    ControlValue _tiltTimerResetActivator;

    TiltedAxisModifier _mod1;
    TiltedAxisModifier _mod2;
    TiltedAxisModifier _mod3;
    TiltedAxisModifier _modFull;

    void resolveActivators();
};



void CombinedModAxis::update()
{
    ModdedAxis::update();

    _twoButtonController.update();
    _mod1Activator.update();
    _mod2Activator.update();
    _mod3Activator.update();
    _modFullActivator.update();
    _tiltTimerResetActivator.update();
    _mod1.update();
    _mod2.update();
    _mod3.update();
    _modFull.update();

    resolveActivators();
}

void CombinedModAxis::resolveActivators()
{
    _mod1Activator = _mod1Control->isActive()
                  && !_mod2Control->isActive();

    _mod2Activator = !_mod1Control->isActive()
                  && _mod2Control->isActive();

    _mod3Activator = _mod1Control->isActive()
                  && _mod2Control->isActive();

    _modFullActivator = !_mod1Control->isActive()
                     && !_mod2Control->isActive();

    _tiltTimerResetActivator = !_twoButtonController.isActive();
}

CombinedModAxis::CombinedModAxis(ControlValue& lowControl,
                                 ControlValue& highControl,
                                 ControlValue& mod1Control,
                                 ControlValue& mod2Control,
                                 ControlValue& tiltControl,
                                 ControlValue& tiltTempDisableControl)
: ModdedAxis(_twoButtonController),
  _twoButtonController(lowControl, highControl),
  _mod1Control(&mod1Control),
  _mod2Control(&mod2Control),
  _mod1(0.30, _mod1Activator, tiltControl, tiltTempDisableControl, _tiltTimerResetActivator),
  _mod2(0.35, _mod2Activator, tiltControl, tiltTempDisableControl, _tiltTimerResetActivator),
  _mod3(0.70, _mod3Activator, tiltControl, tiltTempDisableControl, _tiltTimerResetActivator),
  _modFull(1.0, _modFullActivator, tiltControl, tiltTempDisableControl, _tiltTimerResetActivator)
{
    addModifier(_mod1);
    addModifier(_mod2);
    addModifier(_mod3);
    addModifier(_modFull);
}

#endif // COMBINEDMODAXIS_H
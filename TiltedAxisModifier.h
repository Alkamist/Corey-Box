#ifndef TILTEDAXISMODIFIER_H
#define TILTEDAXISMODIFIER_H

#include "AxisModifier.h"
#include "Timer.h"

class TiltedAxisModifier : public AxisModifier
{
public:
    TiltedAxisModifier(double value,
                       ControlValue& activator,
                       ControlValue& tiltControl,
                       ControlValue& tiltTempDisableControl,
                       ControlValue& tiltTimerResetControl);

    void update();

    virtual void applyModifier(ControlValue& axis);

private:
    AxisModifier _tiltMod;

    ControlValue* _tiltControl;
    ControlValue* _tiltTempDisableControl;
    ControlValue* _tiltTimerResetControl;

    unsigned int _tiltTime;
    unsigned int _tiltTempDisableTime;

    Timer _tiltTimer;
    Timer _tiltTempDisableTimer;

    void initTiltTimes();
};



void TiltedAxisModifier::update()
{
    AxisModifier::update();
    _tiltMod.update();

    if (_tiltTimerResetControl->isActive())
        _tiltTimer.reset();

    if (_tiltTempDisableControl != nullptr)
        if (_tiltTempDisableControl->justActivated())
            _tiltTempDisableTimer.reset();
}

void TiltedAxisModifier::applyModifier(ControlValue& axis)
{
    bool tiltShouldOccur = _tiltControl->isActive()
                        && _tiltTimer.getValue() < _tiltTime
                        && _tiltTempDisableTimer.getValue() > _tiltTempDisableTime;

    if (tiltShouldOccur)
        _tiltMod.applyModifier(axis);
    else
        AxisModifier::applyModifier(axis);
}

void TiltedAxisModifier::initTiltTimes()
{
    _tiltTime = 108;
    _tiltTempDisableTime = 50;
}

TiltedAxisModifier::TiltedAxisModifier(double value,
                                       ControlValue& activator,
                                       ControlValue& tiltControl,
                                       ControlValue& tiltTempDisableControl,
                                       ControlValue& tiltTimerResetControl)
: AxisModifier(value, activator),
  _tiltMod(value * 0.4, activator),
  _tiltControl(&tiltControl),
  _tiltTempDisableControl(&tiltTempDisableControl),
  _tiltTimerResetControl(&tiltTimerResetControl)
{
    initTiltTimes();
}

#endif // TILTEDAXISMODIFIER_H
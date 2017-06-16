#ifndef AXISTILTER_H
#define AXISTILTER_H

#include "AxisClamper.h"

class AxisTilter : public AxisClamper
{
public:
    AxisTilter();

    explicit AxisTilter(const double clampValue,
                        const unsigned int tiltTime,
                        const unsigned int tempDisableTime);

    explicit AxisTilter(const double clampValue,
                        const unsigned int tiltTime,
                        const unsigned int tempDisableTime,
                        const ControlValue& activator,
                        const ControlValue& timerReset,
                        const ControlValue& tempDisabler);

    virtual void update();

    virtual void modify(AnalogAxis& axis) const;

    void setTimerReset(const ControlValue& control)   { _timerReset = &control; }
    void setTempDisabler(const ControlValue& control) { _tempDisabler = &control; }

    const ControlValue& getTimerReset() const         { return *_timerReset; }
    const ControlValue& getTempDisabler() const       { return *_tempDisabler; }

private:
    const ControlValue* _timerReset;
    const ControlValue* _tempDisabler;

    Timer _tiltTimer;
    Timer _tiltTempDisableTimer;
};



void AxisTilter::update()
{
    AxisClamper::update();

    if (_timerReset != nullptr)
        if (_timerReset->isActive())
            _tiltTimer.reset();

    if (_tempDisabler != nullptr)
        if (_tempDisabler->justActivated())
            _tiltTempDisableTimer.reset();
}

void AxisTilter::modify(AnalogAxis& axis) const
{
    bool tiltShouldOccur = getActivator().isActive()
                        && !_tiltTimer.targetTimeReached()
                        && _tiltTempDisableTimer.targetTimeReached();

    if (tiltShouldOccur)
        AxisClamper::modify(axis);
}

AxisTilter::AxisTilter()
: AxisClamper(1.0),
  _tiltTimer(0),
  _tiltTempDisableTimer(0),
  _timerReset(nullptr),
  _tempDisabler(nullptr)
{}

AxisTilter::AxisTilter(const double clampValue,
                       const unsigned int tiltTime,
                       const unsigned int tempDisableTime)
: AxisClamper(clampValue),
  _tiltTimer(tiltTime),
  _tiltTempDisableTimer(tempDisableTime),
  _timerReset(nullptr),
  _tempDisabler(nullptr)
{}

AxisTilter::AxisTilter(const double clampValue,
                       const unsigned int tiltTime,
                       const unsigned int tempDisableTime,
                       const ControlValue& activator,
                       const ControlValue& timerReset,
                       const ControlValue& tempDisabler)
: AxisClamper(clampValue, activator),
  _tiltTimer(tiltTime),
  _tiltTempDisableTimer(tempDisableTime),
  _timerReset(&timerReset),
  _tempDisabler(&tempDisabler)
{}

#endif // AXISTILTER_H
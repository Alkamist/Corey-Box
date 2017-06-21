#ifndef TIMEDACTIVATOR_H
#define TIMEDACTIVATOR_H

#include "ControlValue.h"

class TimedActivator : public ControlValue
{
public:
    TimedActivator();
    explicit TimedActivator(const unsigned int time, const ControlValue& activator);
    explicit TimedActivator(const unsigned int time,
                            const ControlValue& activator,
                            const ControlValue& timerReset);

    virtual void update();

    void setTime(const unsigned int time)              { _timer.setTargetTime(time); }
    void setActivator(const ControlValue& activator)   { _activator = &activator; }
    void setTimerReset(const ControlValue& timerReset) { _timerReset = &timerReset; }

private:
    const ControlValue* _activator;
    const ControlValue* _timerReset;

    Timer _timer;
};



void TimedActivator::update()
{
    ControlValue::update();

    if (_timerReset != nullptr)
        if (_timerReset->justActivated())
            _timer.reset();
    if (_timerReset == nullptr)
        if (_activator->justActivated())
            _timer.reset();

    setValue(_activator->isActive() && !_timer.targetTimeReached());
}

TimedActivator::TimedActivator()
: ControlValue(),
  _timer(0),
  _activator(nullptr),
  _timerReset(nullptr)
{}

TimedActivator::TimedActivator(const unsigned int time, const ControlValue& activator)
: ControlValue(),
  _timer(time),
  _activator(&activator),
  _timerReset(nullptr)
{}

TimedActivator::TimedActivator(const unsigned int time,
                               const ControlValue& activator,
                               const ControlValue& timerReset)
: ControlValue(),
  _timer(time),
  _activator(&activator),
  _timerReset(&timerReset)
{}

#endif // TIMEDACTIVATOR_H
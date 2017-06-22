#ifndef TIMEDACTIVATOR_H
#define TIMEDACTIVATOR_H

#include "Control.h"
#include "ControlSlot.h"

class TimedActivator : public Control
{
public:
    TimedActivator();
    explicit TimedActivator(const unsigned int time, const Control& activator);
    explicit TimedActivator(const unsigned int time,
                            const Control& activator,
                            const Control& timerReset);

    virtual void update();

    void setTime(const unsigned int time)         { _timer.setTargetTime(time); }
    void setActivator(const Control& activator)   { _activator.setControl(activator); }
    void setTimerReset(const Control& timerReset) { _timerReset.setControl(timerReset); }

private:
    ControlSlot _activator;
    ControlSlot _timerReset;

    Timer _timer;
};



void TimedActivator::update()
{
    Control::update();

    if (_timerReset.justActivated())
        _timer.reset();
    if (_activator.justActivated())
        _timer.reset();

    setValue(_activator.isActive() && !_timer.targetTimeReached());
}

TimedActivator::TimedActivator()
: Control(),
  _timer(0),
  _activator(),
  _timerReset()
{}

TimedActivator::TimedActivator(const unsigned int time, const Control& activator)
: Control(),
  _timer(time),
  _activator(activator),
  _timerReset()
{}

TimedActivator::TimedActivator(const unsigned int time,
                               const Control& activator,
                               const Control& timerReset)
: Control(),
  _timer(time),
  _activator(activator),
  _timerReset(timerReset)
{}

#endif // TIMEDACTIVATOR_H
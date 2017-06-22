#ifndef TEMPORARYACTIVATOR_
#define TEMPORARYACTIVATOR_

#include "Control.h"
#include "ControlSlot.h"

class TemporaryActivator : public Control
{
public:
    TemporaryActivator();
    explicit TemporaryActivator(const unsigned int time, const Control& activator);

    virtual void update();

    void setTime(const unsigned int time)         { _timer.setTargetTime(time); }
    void resetTimer()                             { _timer.reset(); }
    void setActivator(const Control& activator)   { _activator.setControl(activator); }

private:
    ControlSlot _activator;

    Timer _timer;
};



void TemporaryActivator::update()
{
    Control::update();

    if (_activator.justActivated())
        _timer.reset();

    setValue(!_timer.targetTimeReached());
}

TemporaryActivator::TemporaryActivator()
: Control(),
  _timer(0),
  _activator()
{}

TemporaryActivator::TemporaryActivator(const unsigned int time, const Control& activator)
: Control(),
  _timer(time),
  _activator(activator)
{}

#endif // TEMPORARYACTIVATOR_
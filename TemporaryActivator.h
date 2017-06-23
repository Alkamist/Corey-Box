#ifndef TEMPORARYACTIVATOR_
#define TEMPORARYACTIVATOR_

#include "Control.h"
#include "Timer.h"

class TemporaryActivator : public Control
{
public:
    TemporaryActivator()
    : Control(),
      _timer(0)
    {}

    TemporaryActivator(const unsigned int time)
    : Control(),
      _timer(time)
    {}

    virtual void update();

    void setControls(const bool control)
    {
        _activator = control;

        if (_activator.justActivated())
            _timer.reset();

        setValue(!_timer.targetTimeReached());
    }

    void setTime(const unsigned int time) { _timer.setTargetTime(time); }

private:
    ControlState _activator;

    Timer _timer;
};



void TemporaryActivator::update()
{
    Control::update();
    _activator.update();
}

#endif // TEMPORARYACTIVATOR_
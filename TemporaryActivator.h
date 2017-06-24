#ifndef TEMPORARYACTIVATOR_
#define TEMPORARYACTIVATOR_

#include "Control.h"
#include "Timer.h"

class TemporaryActivator : public Control
{
public:
    TemporaryActivator()
    : Control(),
      _time(0)
    {}

    TemporaryActivator(const uint64_t time)
    : Control(),
      _time(time)
    {}

    virtual void update()
    {
        Control::update();
        _activator.update();
    }

    void setControls(const bool control)
    {
        _activator = control;

        if (_activator.justActivated())
            _timer.reset();

        setValue(!(_timer >= _time));
    }

    void setTime(const uint64_t time) { _timer = time; }

private:
    ControlState _activator;

    Timer _timer;

    uint64_t _time;
};

#endif // TEMPORARYACTIVATOR_
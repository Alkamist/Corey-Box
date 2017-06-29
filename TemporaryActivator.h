#ifndef TEMPORARYACTIVATOR_
#define TEMPORARYACTIVATOR_

#include "Activator.h"
#include "Timer.h"

class TemporaryActivator : public Activator
{
public:
    TemporaryActivator()
    : Activator(),
      _activator(false),
      _time(0)
    {}

    void process()
    {
        if (_activator)
        {
            setState(true);
            _timer.reset();
        }

        if (_timer >= _time)
            setState(false);
    }

    void setActivatorState(const bool state) { _activator = state; }
    void setTime(const uint64_t time)        { _time = time; }

private:
    bool _activator;

    Timer _timer;

    uint64_t _time;
};

#endif // TEMPORARYACTIVATOR_

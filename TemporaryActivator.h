#ifndef TEMPORARYACTIVATOR_
#define TEMPORARYACTIVATOR_

#include "Activator.h"
#include "Timer.h"

class TemporaryActivator : public Activator
{
public:
    explicit TemporaryActivator(const uint64_t time, const Activator& activator)
    : Activator(),
      _activator(activator),
      _time(time)
    {}

    void process()
    {
        if (_activator.justActivated())
        {
            setState(true);
            _timer.reset();
        }

        if (_timer >= _time)
            setState(false);
    }

    void setTime(const uint64_t time) { _time = time; }

private:
    const Activator& _activator;

    Timer _timer;

    uint64_t _time;
};

#endif // TEMPORARYACTIVATOR_

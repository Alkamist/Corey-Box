#ifndef TIMEDACTIVATOR_H
#define TIMEDACTIVATOR_H

#include "Activator.h"
#include "Timer.h"

// This class is an Activator that needs to be constantly activated
// for a set period of time to activate itself.
class TimedActivator : public Activator
{
public:
    TimedActivator()
    : Activator(),
      _activator(false)
    {}

    void process()
    {
        if (!_activator)
        {
            Activator::operator=(false);
            _timer.reset();
        }

        if (_timer)
            Activator::operator=(true);
    }

    void setTime(const uint8_t time)                     { _timer.setTargetTime(time); }

    virtual TimedActivator& operator =(const bool value) { _activator = value; return *this; }

private:
    bool _activator;

    Timer _timer;
};

#endif // TIMEDACTIVATOR_H

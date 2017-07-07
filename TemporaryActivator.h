#ifndef TEMPORARYACTIVATOR_H
#define TEMPORARYACTIVATOR_H

#include "Activator.h"
#include "Timer.h"

// This class is simply an Activator that is active for a defined period
// of time.
class TemporaryActivator : public Activator
{
public:
    TemporaryActivator()
    : Activator(),
      _activator(false)
    {}

    void process()
    {
        if (_activator)
        {
            Activator::operator=(true);
            _timer.reset();
        }

        if (_timer)
            Activator::operator=(false);
    }

    void setTime(const uint16_t time)                      { _timer.setTargetTime(time); }

    TemporaryActivator& operator =(const bool value)       { _activator = value; return *this; }

private:
    bool _activator;

    Timer _timer;
};

#endif // TEMPORARYACTIVATOR_H

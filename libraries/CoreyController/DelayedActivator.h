#ifndef DELAYEDACTIVATOR_H
#define DELAYEDACTIVATOR_H

#include "TemporaryActivator.h"

class DelayedActivator : public Activator
{
public:
    DelayedActivator()
    : Activator(),
      _delayCondition(false),
      _delayActivator(false)
    {
        _delay.setTime(3);
        _minimumActivationTime.setTime(3);
    }

    void process()
    {
        _delay = _delayCondition && !_activator;
        _delay.process();

        if (_delay && _activator.justActivated())
            _delayActivator = true;

        _minimumActivationTime = _delay.justDeactivated() && _delayActivator;
        _minimumActivationTime.process();

        if (_delay.justDeactivated())
            _delayActivator = false;

        Activator::operator=((_activator && !_delay) || _minimumActivationTime);
    }

    void endCycle()
    {
        Activator::endCycle();
        _delay.endCycle();
        _minimumActivationTime.endCycle();
        _activator.endCycle();
    }

    void setDelayCondition(const bool state)               { _delayCondition = state; }
    void setDelayTime(const uint8_t value)                 { _delay.setTime(value); }

    virtual DelayedActivator& operator =(const bool value) { _activator = value; return *this; }

private:
    Activator _activator;
    bool _delayCondition;
    bool _delayActivator;

    TemporaryActivator _delay;
    TemporaryActivator _minimumActivationTime;
};

#endif // DELAYEDACTIVATOR_H

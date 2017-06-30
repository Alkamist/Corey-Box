#ifndef TOGGLEACTIVATOR_H
#define TOGGLEACTIVATOR_H

#include "Activator.h"

// This class is an activator that toggles on or off when it gets
// a high signal from an activator.
class ToggleActivator : public Activator
{
public:
    ToggleActivator()
    : Activator(),
      _activator(false)
    {}

    explicit ToggleActivator(const bool state)
    : Activator(state),
      _activator(state)
    {}

    void process()
    {
        if (_activator && !isActive())
        {
            setState(true);
            return;
        }

        if (_activator && isActive())
        {
            setState(false);
            return;
        }
    }

    void setActivatorState(const bool state)
    {
        _activator = state;
    }

private:
    bool _activator;
};

#endif // TOGGLEACTIVATOR_H

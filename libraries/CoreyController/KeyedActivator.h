#ifndef KEYEDACTIVATOR_H
#define KEYEDACTIVATOR_H

#include "Activator.h"

class KeyedActivator : public Activator
{
public:
    KeyedActivator()
    : Activator(),
      _keepHolding(false),
      _initialHold(false)
    {}

    void process()
    {
        _initialHold = _key && _hold;

        if (_key.justDeactivated() && _hold) _keepHolding = true;

        if (!_hold) _keepHolding = false;

        setState(_keepHolding || _initialHold);
    }

    void endCycle()
    {
        Activator::endCycle();
        _key.endCycle();
        _hold.endCycle();
    }

    void setKeyState(const bool state)                   { _key = state; }
    void setHoldState(const bool state)                  { _hold = state; }

    //virtual KeyedActivator& operator =(const bool value) { Activator::operator=(value); return *this; }

private:
    Activator _key;
    Activator _hold;

    bool _keepHolding;
    bool _initialHold;
};

#endif // KEYEDACTIVATOR_H

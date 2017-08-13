#ifndef JUMPMANAGER_H
#define JUMPMANAGER_H

#include "Activator.h"
#include "TemporaryActivator.h"
#include "KeyedActivator.h"

// This class handles the short hop and full hop buttons. It allows for double jumps with
// either button. Be sure to use the full hop button for floating (Peach etc...).
class JumpManager : public Activator
{
public:
    JumpManager()
    : Activator()
    {
        _yHold.setTime(4);
        _xHold.setTime(18);
        _xSafety.setTime(22);
    }

    void process()
    {
        _yAlternate.setKeyState(_xSafety && _fullHop.justActivated());
        _yAlternate.setHoldState(_fullHop);
        _yAlternate.process();

        _yHold = (_yAlternate.justActivated() || _shortHop.justActivated()) && !_yHold;
        _yHold.process();

        bool xCondition = _fullHop.justActivated() && !_xSafety;
        _xHold = xCondition;
        _xSafety = xCondition;
        _xHold.process();
        _xSafety.process();
    }

    void endCycle()
    {
        _shortHop.endCycle();
        _fullHop.endCycle();
        _yAlternate.endCycle();
        _xHold.endCycle();
        _yHold.endCycle();
        _xSafety.endCycle();
    }

    const bool getXState() const                     { return _xHold || _fullHop; }
    const bool getYState() const                     { return _yHold; }

    void setShortHopState(const bool state)          { _shortHop = state; }
    void setFullHopState(const bool state)           { _fullHop = state; }

    virtual JumpManager& operator=(const bool value) { setState(value); return *this; }

private:
    Activator _shortHop;
    Activator _fullHop;

    KeyedActivator _yAlternate;

    TemporaryActivator _xHold;
    TemporaryActivator _yHold;
    TemporaryActivator _xSafety;
};

#endif // JUMPMANAGER_H

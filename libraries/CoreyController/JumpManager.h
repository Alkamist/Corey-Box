#ifndef JUMPMANAGER_H
#define JUMPMANAGER_H

#include "Activator.h"
#include "TemporaryActivator.h"
#include "KeyedActivator.h"

class JumpManager : public Activator
{
public:
    JumpManager()
    : Activator()
    {
        _yHold.setTime(2);
        _xHold.setTime(9);
    }

    void process()
    {
        _yAlternate.setKeyState(_xHold && _fullHop.justActivated());
        _yAlternate.setHoldState(_fullHop);
        _yAlternate.process();

        _yHold = (_yAlternate.justActivated() || _shortHop.justActivated()) && !_yHold;
        _yHold.process();

        _xHold = _fullHop.justActivated() && !_xHold;
        _xHold.process();
    }

    void endCycle()
    {
        _shortHop.endCycle();
        _fullHop.endCycle();
        _yAlternate.endCycle();
        _xHold.endCycle();
        _yHold.endCycle();
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
};

#endif // JUMPMANAGER_H

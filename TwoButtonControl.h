#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "Control.h"
#include "TwoButtonStateTracker.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonControl : public Control
{
public:
    TwoButtonControl()
    : Control()
    {
        makeBipolar();
    }

    virtual void process();

    void setLowState(const bool state)  { _stateTracker.setState1(state); }
    void setHighState(const bool state) { _stateTracker.setState2(state); }

private:
    TwoButtonStateTracker _stateTracker;
};



void TwoButtonControl::process()
{
    _stateTracker.process();

    if (_stateTracker.getState1() && _stateTracker.state2WasFirst())
    {
        setValue(-1.0);
        return;
    }

    if (_stateTracker.getState1() && !_stateTracker.getState2())
    {
        setValue(-1.0);
        return;
    }

    if (_stateTracker.state1WasFirst() && _stateTracker.getState2())
    {
        setValue(1.0);
        return;
    }

    if (!_stateTracker.getState1() && _stateTracker.getState2())
    {
        setValue(1.0);
        return;
    }

    if (!_stateTracker.getState1() && !_stateTracker.getState2())
    {
        setValue(0.0);
        return;
    }
}

#endif // TWOBUTTONCONTROL_H
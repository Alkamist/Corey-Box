#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "Control.h"
#include "TwoButtonStateTracker.h"

// This class is responsible for the base logic of how two buttons interact with
// a joystick axis. The most recently pushed button gets priority.
class TwoButtonControl : public Control
{
public:
    TwoButtonControl()
    : Control(128),
      _range(127)
    {}

    virtual void process();

    void setLowState(const bool state)         { _stateTracker.setState1(state); }
    void setHighState(const bool state)        { _stateTracker.setState2(state); }

    virtual void setRange(const uint8_t value) { _range = value; }

private:
    uint8_t _range;

    TwoButtonStateTracker _stateTracker;

    TwoButtonControl& operator =(const uint8_t value)  { Control::operator=(value); return *this; }
};



void TwoButtonControl::process()
{
    _stateTracker.process();

    if (_stateTracker.getState1() && _stateTracker.state2WasFirst())
    {
        *this = 128 - _range;
        return;
    }

    if (_stateTracker.getState1() && !_stateTracker.getState2())
    {
        *this = 128 - _range;
        return;
    }

    if (_stateTracker.state1WasFirst() && _stateTracker.getState2())
    {
        *this = 128 + _range;
        return;
    }

    if (!_stateTracker.getState1() && _stateTracker.getState2())
    {
        *this = 128 + _range;
        return;
    }

    if (!_stateTracker.getState1() && !_stateTracker.getState2())
    {
        *this = 128;
        return;
    }
}

#endif // TWOBUTTONCONTROL_H
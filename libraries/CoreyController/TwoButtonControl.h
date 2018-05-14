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
    : Control(128, 127)
    {}

    explicit TwoButtonControl(const uint8_t range)
    : Control(128, range)
    {}

    void process()
    {
        _stateTracker.process();

        if (_stateTracker.getState1() && _stateTracker.state2WasFirst())
        {
            setToMinimum();
            return;
        }

        if (_stateTracker.getState1() && !_stateTracker.getState2())
        {
            setToMinimum();
            return;
        }

        if (_stateTracker.state1WasFirst() && _stateTracker.getState2())
        {
            setToMaximum();
            return;
        }

        if (!_stateTracker.getState1() && _stateTracker.getState2())
        {
            setToMaximum();
            return;
        }

        if (!_stateTracker.getState1() && !_stateTracker.getState2())
        {
            setToCenter();
            return;
        }
    }

    void setLowState(const bool state)  { _stateTracker.setState1(state); }
    void setHighState(const bool state) { _stateTracker.setState2(state); }

private:
    TwoButtonStateTracker _stateTracker;

    TwoButtonControl& operator =(const uint8_t value)  { Control::operator=(value); return *this; }
};

#endif // TWOBUTTONCONTROL_H
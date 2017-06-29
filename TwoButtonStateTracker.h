#ifndef TWOBUTTONSTATETRACKER_H
#define TWOBUTTONSTATETRACKER_H

class TwoButtonStateTracker
{
public:
    TwoButtonStateTracker()
    : _state1(false),
      _state2(false),
      _state1WasFirst(false),
      _state2WasFirst(false)
    {}

    virtual void process();

    void setState1(const bool state)  { _state1 = state; }
    void setState2(const bool state)  { _state2 = state; }

    const bool getState1() const      { return _state1; }
    const bool getState2() const      { return _state2; }
    const bool state1WasFirst() const { return _state1WasFirst; }
    const bool state2WasFirst() const { return _state2WasFirst; }

private:
    bool _state1;
    bool _state2;

    bool _state1WasFirst;
    bool _state2WasFirst;
};



void TwoButtonStateTracker::process()
{
    if (_state1 && !_state2)
    {
        _state1WasFirst = true;
        _state2WasFirst = false;
        return;
    }

    if (!_state1 && _state2)
    {
        _state1WasFirst = false;
        _state2WasFirst = true;
        return;
    }

    if (!_state1 && !_state2)
    {
        _state1WasFirst = false;
        _state2WasFirst = false;
        return;
    }
}

#endif // TWOBUTTONSTATETRACKER_H
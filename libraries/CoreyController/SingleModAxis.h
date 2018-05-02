#ifndef SINGLEMODAXIS_H
#define SINGLEMODAXIS_H

#include "TwoButtonControl.h"
#include "ScaleBipolar.h"

// This has a TwoButtonControl that can be modified by a fixed mod value
// when told to do so.
class SingleModAxis : public Control
{
public:
    SingleModAxis()
    : Control(128),
      _modState(false),
      _modValue(52),
      _range(127)
    {}

    void process()
    {
        _twoButtonControl.process();

        if (_modState)
        {
            *this = scaleBipolar(_twoButtonControl, _modValue);
            return;
        }

        *this = scaleBipolar(_twoButtonControl, _range);
    }

    void endCycle()
    {
        Control::endCycle();
        _twoButtonControl.endCycle();
    }

    void setLowState(const bool state)    { _twoButtonControl.setLowState(state); }
    void setHighState(const bool state)   { _twoButtonControl.setHighState(state); }
    void setModState(const bool state)    { _modState = state; }

    void setModValue(const uint8_t value) { _modValue = value; }

    void setRange(const uint8_t value)    { _range = value; }

    const int8_t getRange() const         { return _range; }
    const int8_t getModValue() const      { return _modValue; }

private:
    bool _modState;
    uint8_t _modValue;

    uint8_t _range;

    TwoButtonControl _twoButtonControl;

    SingleModAxis& operator =(const uint8_t value)  { Control::operator=(value); return *this; }
};

#endif // SINGLEMODAXIS_H

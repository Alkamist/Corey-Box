#ifndef SINGLEMODAXIS_H
#define SINGLEMODAXIS_H

#include "TwoButtonControl.h"
#include "ScaleBipolar.h"

// This is a TwoButtonControl that can be modified by a fixed mod value
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

    void setLowState(const bool state)  { _twoButtonControl.setLowState(state); }
    void setHighState(const bool state) { _twoButtonControl.setHighState(state); }
    void setModState(const bool state)  { _modState = state; }

    void setRange(const uint8_t value)  { _range = value; }

private:
    bool _modState;
    uint8_t _modValue;

    uint8_t _range;

    TwoButtonControl _twoButtonControl;

    SingleModAxis& operator =(const uint8_t value)  { Control::operator=(value); return *this; }
};

#endif // SINGLEMODAXIS_H

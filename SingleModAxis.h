#ifndef SINGLEMODAXIS_H
#define SINGLEMODAXIS_H

#include "TwoButtonControl.h"

// This is a TwoButtonControl that can be modified by a fixed mod value
// when told to do so
class SingleModAxis : public Control
{
public:
    SingleModAxis()
    : Control(),
      _modState(false),
      _modValue(0.65)
    {
        makeBipolar();
    }

    void process()
    {
        _twoButtonControl.process();

        if (_modState)
        {
            setValue(_twoButtonControl.getValue() * _modValue);
            return;
        }

        setValue(_twoButtonControl.getValue());
    }

    void endCycle()
    {
        Control::endCycle();
        _twoButtonControl.endCycle();
    }

    void setLowState(const bool state)  { _twoButtonControl.setLowState(state); }
    void setHighState(const bool state) { _twoButtonControl.setHighState(state); }
    void setModState(const bool state)  { _modState = state; }

private:
    bool _modState;

    TwoButtonControl _twoButtonControl;

    const float _modValue;
};

#endif // SINGLEMODAXIS_H

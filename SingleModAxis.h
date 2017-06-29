#ifndef SINGLEMODAXIS_H
#define SINGLEMODAXIS_H

#include "TwoButtonControl.h"

class SingleModAxis : public BipolarControl
{
public:
    SingleModAxis()
    : BipolarControl(),
      _modState(false),
      _modValue(0.65)
    {}

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
        BipolarControl::endCycle();
        _twoButtonControl.endCycle();
    }

    void setLowState(const bool state)  { _twoButtonControl.setLowState(state); }
    void setHighState(const bool state) { _twoButtonControl.setHighState(state); }
    void setModState(const bool state)  { _modState = state; }

private:
    bool _modState;

    TwoButtonControl _twoButtonControl;

    const double _modValue;
};

#endif // SINGLEMODAXIS_H

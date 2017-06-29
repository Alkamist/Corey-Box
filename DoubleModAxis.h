#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"

class DoubleModAxis : public BipolarControl
{
public:
    DoubleModAxis()
    : BipolarControl(),
      _mod1State(false),
      _mod2State(false),
      _mod1Value(0.33250),
      _mod2Value(0.52500),
      _mod3Value(0.76250)
    {}

    void process()
    {
        _twoButtonControl.process();

        if (_mod1State && !_mod2State)
        {
            setValue(_twoButtonControl.getValue() * _mod1Value);
            return;
        }

        if (!_mod1State && _mod2State)
        {
            setValue(_twoButtonControl.getValue() * _mod2Value);
            return;
        }

        if (_mod1State && _mod2State)
        {
            setValue(_twoButtonControl.getValue() * _mod3Value);
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
    void setMod1State(const bool state) { _mod1State = state; }
    void setMod2State(const bool state) { _mod2State = state; }

    void trimModsOutward()
    {
        if (_mod3Value > 0.8500)
            return;

        _mod1Value += 0.015;
        _mod2Value += 0.015;
        _mod3Value += 0.015;
    }

    void trimModsInward()
    {
        if (_mod1Value < 0.2850)
            return;

        _mod1Value -= 0.015;
        _mod2Value -= 0.015;
        _mod3Value -= 0.015;
    }

    void resetMods()
    {
        _mod1Value = 0.33250;
        _mod2Value = 0.52500;
        _mod3Value = 0.76250;
    }

private:
    bool _mod1State;
    bool _mod2State;

    TwoButtonControl _twoButtonControl;

    double _mod1Value;
    double _mod2Value;
    double _mod3Value;
};

#endif // DOUBLEMODAXIS_H

#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"

// This class is a TwoButtonControl that can be modded by two mod buttons.
// Either mod and the combination of both give different mod values.
class DoubleModAxis : public Control
{
public:
    DoubleModAxis()
    : Control(),
      _mod1State(false),
      _mod2State(false),
      _mod1Value(0.0),
      _mod2Value(0.0),
      _mod3Value(0.0)
    {
        resetMods();
        makeBipolar();
    }

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
        Control::endCycle();
        _twoButtonControl.endCycle();
    }

    void setLowState(const bool state)  { _twoButtonControl.setLowState(state); }
    void setHighState(const bool state) { _twoButtonControl.setHighState(state); }
    void setMod1State(const bool state) { _mod1State = state; }
    void setMod2State(const bool state) { _mod2State = state; }

    const float getMod1Value() const    { return _mod1Value; }

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

    void setModStart(const float value)
    {
        float modAdditive = (1.0 - value) / 3.0;

        _mod1Value = value;
        _mod2Value = value + modAdditive;
        _mod3Value = value + 2.0 * modAdditive;
    }

    void resetMods()
    {
        setModStart(0.33250);
    }

private:
    bool _mod1State;
    bool _mod2State;

    TwoButtonControl _twoButtonControl;

    float _mod1Value;
    float _mod2Value;
    float _mod3Value;
};

#endif // DOUBLEMODAXIS_H

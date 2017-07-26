#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"
#include "ScaleBipolar.h"

// This class is a TwoButtonControl that can be modded by two mod buttons.
// Either mod and the combination of both give different mod values.
class DoubleModAxis : public Control
{
public:
    DoubleModAxis()
    : Control(128),
      _mod1State(false),
      _mod2State(false),
      _mod1Value(0),
      _mod2Value(0),
      _mod3Value(0),
      _range(127)
    {
        setModStart(25);
    }

    void process()
    {
        _twoButtonControl.process();

        if (_mod1State && !_mod2State)
        {
            *this = scaleBipolar(_twoButtonControl, _mod1Value);
            return;
        }

        if (!_mod1State && _mod2State)
        {
            *this = scaleBipolar(_twoButtonControl, _mod2Value);
            return;
        }

        if (_mod1State && _mod2State)
        {
            *this = scaleBipolar(_twoButtonControl, _mod3Value);
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
    void setMod1State(const bool state) { _mod1State = state; }
    void setMod2State(const bool state) { _mod2State = state; }

    const int8_t getMod1Value() const   { return _mod1Value; }

    void setRange(const uint8_t value)  { _range = value; }

    void setModStart(const uint8_t value)
    {
        uint8_t modAdditive = (_range - value) / 3;

        _mod1Value = value;
        _mod2Value = value + modAdditive;
        _mod3Value = value + 2 * modAdditive;
    }

private:
    bool _mod1State;
    bool _mod2State;

    uint8_t _mod1Value;
    uint8_t _mod2Value;
    uint8_t _mod3Value;

    uint8_t _range;

    TwoButtonControl _twoButtonControl;

    DoubleModAxis& operator =(const uint8_t value)  { Control::operator=(value); return *this; }
};

#endif // DOUBLEMODAXIS_H

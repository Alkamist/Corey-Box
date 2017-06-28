#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"

class DoubleModAxis : public BipolarControl
{
public:
    DoubleModAxis(const Activator& low, const Activator& high,
                  const Activator& mod1, const Activator& mod2)
    : BipolarControl(),
      _mod1(mod1),
      _mod2(mod2),
      _twoButtonControl(low, high),
      _mod1Value(0.33250),
      _mod2Value(0.52500),
      _mod3Value(0.76250)
    {}

    void process()
    {
        _twoButtonControl.process();

        if (_mod1 && !_mod2)
        {
            setValue(_twoButtonControl.getValue() * _mod1Value);
            return;
        }

        if (!_mod1 && _mod2)
        {
            setValue(_twoButtonControl.getValue() * _mod2Value);
            return;
        }

        if (_mod1 && _mod2)
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
    const Activator& _mod1;
    const Activator& _mod2;

    TwoButtonControl _twoButtonControl;

    double _mod1Value;
    double _mod2Value;
    double _mod3Value;
};

#endif // DOUBLEMODAXIS_H

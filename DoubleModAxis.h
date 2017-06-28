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
      _mod1Value(0.33750),
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

private:
    const Activator& _mod1;
    const Activator& _mod2;

    TwoButtonControl _twoButtonControl;

    const double _mod1Value;
    const double _mod2Value;
    const double _mod3Value;
};

#endif // DOUBLEMODAXIS_H

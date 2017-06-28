#ifndef SINGLEMODAXIS_H
#define SINGLEMODAXIS_H

#include "TwoButtonControl.h"

class SingleModAxis : public BipolarControl
{
public:
    SingleModAxis(const Activator& low, const Activator& high, const Activator& mod)
    : BipolarControl(),
      _mod(mod),
      _twoButtonControl(low, high),
      _modValue(0.65)
    {}

    void process()
    {
        _twoButtonControl.process();

        if (_mod)
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

private:
    const Activator& _mod;

    TwoButtonControl _twoButtonControl;

    const double _modValue;
};

#endif // SINGLEMODAXIS_H

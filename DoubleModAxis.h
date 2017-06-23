#ifndef DOUBLEMODAXIS_H
#define DOUBLEMODAXIS_H

#include "TwoButtonControl.h"
#include "Control.h"

class DoubleModAxis : public Control
{
public:
    DoubleModAxis()
    : Control(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _mod1Value(0.34),
      _mod2Value(0.53),
      _mod3Value(0.76)
    {}

    virtual void update()
    {
        Control::update();
        _twoButtonControl.update();
    }

    virtual void setControls(const bool low, const bool high,
                             const bool mod1, const bool mod2)
    {
        _twoButtonControl.setControls(low, high);

        if (!mod1 && !mod2)
            setValue(_twoButtonControl.getValue());

        if (mod1 && !mod2)
            setValue(_mod1Value * _twoButtonControl.getValue());

        if (!mod1 && mod2)
            setValue(_mod2Value * _twoButtonControl.getValue());

        if (mod1 && mod2)
            setValue(_mod3Value * _twoButtonControl.getValue());
    }
private:
    TwoButtonControl _twoButtonControl;

    const double _mod1Value;
    const double _mod2Value;
    const double _mod3Value;
};

#endif // DOUBLEMODAXIS_H

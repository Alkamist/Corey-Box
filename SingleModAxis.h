#ifndef SINGLEMODAXIS_H
#define SINGLEMODAXIS_H

#include "TwoButtonControl.h"
#include "Control.h"

class SingleModAxis : public Control
{
public:
    SingleModAxis()
    : Control(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _modValue(0.65)
    {}

    virtual void update()
    {
        Control::update();
        _twoButtonControl.update();
    }

    virtual void setControls(const bool low, const bool high,
                             const bool mod)
    {
        _twoButtonControl.setControls(low, high);

        if (!mod)
            setValue(_twoButtonControl.getValue());

        if (mod)
            setValue(_modValue * _twoButtonControl.getValue());
    }
private:
    TwoButtonControl _twoButtonControl;

    const double _modValue;
};

#endif // SINGLEMODAXIS_H

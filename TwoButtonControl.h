#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "BipolarControl.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonControl : public BipolarControl
{
public:
    explicit TwoButtonControl(const Activator& low, const Activator& high)
    : BipolarControl(),
      _low(low),
      _high(high),
      _pressOrder(-1)
    {}

    void process();

private:
    const Activator& _low;
    const Activator& _high;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder;

    void runLogic();
};



void TwoButtonControl::process()
{
    if (_low.stateJustChanged() || _high.stateJustChanged())
        runLogic();
}

void TwoButtonControl::runLogic()
{
    bool lowIsPressed = _low.isActive();
    bool highIsPressed = _high.isActive();

    bool lowWasPressedFirst = lowIsPressed && (_pressOrder == 0);
    bool highWasPressedFirst = highIsPressed && (_pressOrder == 1);

    // Low Cases
    if (lowIsPressed && highWasPressedFirst)
    {
        _pressOrder = 1;
        setValue(-1.0);
        return;
    }
    if (lowIsPressed && !highIsPressed)
    {
        _pressOrder = 0;
        setValue(-1.0);
        return;
    }

    // High Cases
    if (lowWasPressedFirst && highIsPressed)
    {
        _pressOrder = 0;
        setValue(1.0);
        return;
    }
    if (!lowIsPressed && highIsPressed)
    {
        _pressOrder = 1;
        setValue(1.0);
        return;
    }

    // Middle Cases
    if (!lowIsPressed && !highIsPressed)
    {
        _pressOrder = -1;
        setValue(0.0);
        return;
    }
    if (lowIsPressed && highIsPressed && (_pressOrder == -1))
    {
        _pressOrder = -1;
        setValue(0.0);
        return;
    }
}

#endif // TWOBUTTONCONTROL_H
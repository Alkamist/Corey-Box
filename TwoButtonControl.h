#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "BipolarControl.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonControl : public BipolarControl
{
public:
    explicit TwoButtonControl()
    : BipolarControl(),
      _lowState(false),
      _highState(false),
      _pressOrder(-1)
    {}

    virtual void process();

    void setLowState(const bool state)  { _lowState = state; }
    void setHighState(const bool state) { _highState = state; }

private:
    bool _lowState;
    bool _highState;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder;
};



void TwoButtonControl::process()
{
    bool lowIsPressed = _lowState;
    bool highIsPressed = _highState;

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
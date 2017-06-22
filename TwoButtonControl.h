#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "Control.h"
#include "ControlSlot.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonControl : public Control
{
public:
    TwoButtonControl(const Control& low, const Control& high)
    : Control(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _pressOrder(-1),
      _lowControl(low),
      _highControl(high)
    {}

    virtual void update();

private:
    ControlSlot _lowControl;
    ControlSlot _highControl;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder;

    void runLogic();
};



void TwoButtonControl::update()
{
    Control::update();

    if (_lowControl.hasChanged() || _highControl.hasChanged())
        runLogic();
}

void TwoButtonControl::runLogic()
{
    bool lowIsPressed = _lowControl.isActive();
    bool highIsPressed = _highControl.isActive();

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
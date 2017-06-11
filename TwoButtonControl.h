#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "ControlValue.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonControl : public ControlValue
{
public:
    TwoButtonControl()
    : ControlValue(0.5, 0.5),
      _pressOrder(-1)
    {}

    TwoButtonControl(ControlValue& lowControl, ControlValue& highControl)
    : ControlValue(0.5, 0.5),
      _lowControl(&lowControl),
      _highControl(&highControl),
      _pressOrder(-1)
    {}

    void update();

    void setLowControl(ControlValue& control)  { _lowControl = &control; }
    void setHighControl(ControlValue& control) { _highControl = &control; }

private:
    ControlValue* _lowControl;
    ControlValue* _highControl;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder;
};



void TwoButtonControl::update()
{
    ControlValue::update();

    bool lowIsPressed = false;
    bool highIsPressed = false;

    if (_lowControl != nullptr)
        lowIsPressed = _lowControl->isActive();

    if (_highControl != nullptr)
        highIsPressed = _highControl->isActive();

    bool lowWasPressedFirst = lowIsPressed && (_pressOrder == 0);
    bool highWasPressedFirst = highIsPressed && (_pressOrder == 1);

    // Low Cases
    if (lowIsPressed && highWasPressedFirst)
    {
        _pressOrder = 1;
        setValue(0.0);
        return;
    }
    if (lowIsPressed && !highIsPressed)
    {
        _pressOrder = 0;
        setValue(0.0);
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
        setValue(0.5);
        return;
    }
    if (lowIsPressed && highIsPressed && (_pressOrder == -1))
    {
        _pressOrder = -1;
        setValue(0.5);
        return;
    }
}

#endif // TWOBUTTONCONTROL_H
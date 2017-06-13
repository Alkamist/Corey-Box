#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "ControlValue.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonControl : public ControlValue
{
public:
    TwoButtonControl();
    explicit TwoButtonControl(const ControlValue& lowControl,
                              const ControlValue& highControl);

    void update();

    void setLowControl(const ControlValue& control)  { _lowControl = &control; }
    void setHighControl(const ControlValue& control) { _highControl = &control; }

private:
    const ControlValue* _lowControl;
    const ControlValue* _highControl;

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

TwoButtonControl::TwoButtonControl()
: ControlValue(0.5, 0.5),
  _lowControl(nullptr),
  _highControl(nullptr),
  _pressOrder(-1)
{}

TwoButtonControl::TwoButtonControl(const ControlValue& lowControl,
                                   const ControlValue& highControl)
: ControlValue(0.5, 0.5),
  _lowControl(&lowControl),
  _highControl(&highControl),
  _pressOrder(-1)
{}

#endif // TWOBUTTONCONTROL_H
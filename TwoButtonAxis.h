#ifndef TWOBUTTONAXIS_H
#define TWOBUTTONAXIS_H

#include "ControlValue.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonAxis : public ControlValue
{
public:
    TwoButtonAxis();

    void update();

    void setLowControl(ControlValue& controlValue)  { _lowControl = &controlValue; }
    void setHighControl(ControlValue& controlValue) { _highControl = &controlValue; }

private:
    ControlValue* _lowControl;
    ControlValue* _highControl;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder;
};



void TwoButtonAxis::update()
{
    ControlValue::update();

    bool lowIsPressed = false;
    bool highIsPressed = false;

    if (_lowControl != nullptr)
    {
        lowIsPressed = _lowControl->isActive();
    }

    if (_highControl != nullptr)
    {
        highIsPressed = _highControl->isActive();
    }

    bool lowWasPressedFirst = lowIsPressed && (_pressOrder == 0);
    bool highWasPressedFirst = highIsPressed && (_pressOrder == 1);

    // Low Cases
    if (lowIsPressed && highWasPressedFirst)
    {
        _pressOrder = 1;
        setValue(0.0);
    }
    else if (lowIsPressed && !highIsPressed)
    {
        _pressOrder = 0;
        setValue(0.0);
    }

    // High Cases
    else if (lowWasPressedFirst && highIsPressed)
    {
        _pressOrder = 0;
        setValue(1.0);
    }
    else if (!lowIsPressed && highIsPressed)
    {
        _pressOrder = 1;
        setValue(1.0);
    }

    // Middle Cases
    else if (!lowIsPressed && !highIsPressed)
    {
        _pressOrder = -1;
        setValue(0.5);
    }
    else if (lowIsPressed && highIsPressed && (_pressOrder == -1))
    {
        _pressOrder = -1;
        setValue(0.5);
    }
}

TwoButtonAxis::TwoButtonAxis()
{
    setValue(0.5);

    DeadZone deadZone;
    deadZone.setCenter(0.5);
    deadZone.setRange(0.1);

    setDeadZone(deadZone);
}

#endif // TWOBUTTONAXIS_H
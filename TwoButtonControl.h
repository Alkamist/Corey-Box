#ifndef TWOBUTTONCONTROL_H
#define TWOBUTTONCONTROL_H

#include "Control.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonControl : public Control
{
public:
    TwoButtonControl()
    : Control(0.0, Range<double>(Bounds<double>(-1.0, 1.0))),
      _pressOrder(-1),
      _lowState(false),
      _highState(false)
    {}

    virtual void update();

    void setLowState(const bool value);
    void setHighState(const bool value);

    void setStates(const bool low, const bool high);

private:
    bool _lowState;
    bool _highState;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder;
};



void TwoButtonControl::update()
{
    Control::update();

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

void TwoButtonControl::setLowState(const bool value)
{
    if (value != _lowState)
        _lowState = value;
}

void TwoButtonControl::setHighState(const bool value)
{
    if (value != _highState)
        _highState = value;
}

void TwoButtonControl::setStates(const bool low, const bool high)
{
    setLowState(low);
    setHighState(high);
}

#endif // TWOBUTTONCONTROL_H
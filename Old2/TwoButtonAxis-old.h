#ifndef TWOBUTTONAXIS_H
#define TWOBUTTONAXIS_H

#include "DigitalButton.h"
#include "AnalogAxis.h"

// This class is responsible for the base logic of how
// two buttons interact with a joystick axis.
class TwoButtonAxis : public AnalogAxis
{
public:
    TwoButtonAxis(DigitalButton& lowButton,
                  DigitalButton& highButton)
     : _pressOrder(-1),
       _lowButton(&lowButton),
       _highButton(&highButton)
    {}

    void update();

private:
    DigitalButton* _lowButton;
    DigitalButton* _highButton;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder;
};



void TwoButtonAxis::update()
{
    bool lowIsPressed = _lowButton.pressed();
    bool highIsPressed = _highButton.pressed();
    bool lowWasPressedFirst = lowIsPressed && (_pressOrder == 0);
    bool highWasPressedFirst = highIsPressed && (_pressOrder == 1);

    // Low Cases
    if (lowIsPressed && highWasPressedFirst)
    {
        _pressOrder = 1;
        setValue(-1.0);
    }
    else if (lowIsPressed && !highIsPressed)
    {
        _pressOrder = 0;
        setValue(-1.0);
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
        setValue(0.0);
    }
    else if (lowIsPressed && highIsPressed && (_pressOrder == -1))
    {
        _pressOrder = -1;
        setValue(0.0);
    }

    AnalogAxis::update();
}

#endif // TWOBUTTONAXIS_H
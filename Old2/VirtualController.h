#ifndef VIRTUALCONTROLLER_H
#define VIRTUALCONTROLLER_H

#include "DigitalButton.h"
#include "AnalogButton.h"
#include "AnalogStick.h"

class VirtualController
{
public:
    VirtualController();

    void update();

private:
    DigitalButton aButton;
    DigitalButton bButton;
    DigitalButton xButton;
    DigitalButton yButton;
    DigitalButton zButton;
    DigitalButton lButton;
    DigitalButton rButton;
    DigitalButton startButton;
    DigitalButton dLeft;
    DigitalButton dRight;
    DigitalButton dDown;
    DigitalButton dUp;

    AnalogButton lAnalog;
    AnalogButton rAnalog;

    AnalogStick lStick;
    AnalogStick cStick;
};

#endif // VIRTUALCONTROLLER_H
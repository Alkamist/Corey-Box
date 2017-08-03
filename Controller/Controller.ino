// This code is meant for a Teensy++ 2.0. It runs all of the controller logic
// and gives the data to the Arduino Nano when requested.

#include <Wire.h>
#include "ButtonOnlyController.h"

void transferController();
const unsigned char boolsToByte(const bool bools[8]);

const uint8_t _slaveAddress = 22;

ButtonOnlyController controller;

void setup()
{
    Wire.begin(_slaveAddress);
    Wire.onRequest(transferController);
}

void loop()
{
    controller.process();
    controller.endCycle();
}

void transferController()
{
    unsigned char output[8];

    bool buttons1[8];
    bool buttons2[8];

    buttons1[0] = controller.a;
    buttons1[1] = controller.b;
    buttons1[2] = controller.x;
    buttons1[3] = controller.y;
    buttons1[4] = controller.z;
    buttons1[5] = controller.l;
    buttons1[6] = controller.r;
    buttons1[7] = controller.start;

    buttons2[0] = controller.dLeft;
    buttons2[1] = controller.dRight;
    buttons2[2] = controller.dDown;
    buttons2[3] = controller.dUp;
    buttons2[4] = false;
    buttons2[5] = false;
    buttons2[6] = false;
    buttons2[7] = false;

    output[0] = boolsToByte(buttons1);
    output[1] = boolsToByte(buttons2);
    output[2] = controller.lsX;
    output[3] = controller.lsY;
    output[4] = controller.cX;
    output[5] = controller.cY;
    output[6] = controller.lAnalog;
    output[7] = controller.rAnalog;

    Wire.write(output, 8);
}

const unsigned char boolsToByte(const bool bools[8])
{
    unsigned char output = 0;

    for (int index = 0; index < 8; ++index)
        if (bools[index])
            output |= 1 << index;

    return output;
}
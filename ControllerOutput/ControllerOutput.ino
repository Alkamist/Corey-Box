// This code is meant for an Arduino Nano. Plug in SDA (Pin A4) and SCL (Pin A5).
// The code requests 8 bytes from another microcontroller via the Wire library,
// and then writes them to the GameCube on Pin D2. Make sure to connect the
// Arduino to a logic level converter before it goes to the GameCube.
// It will work without it on Dolphin but it will disconnect randomly on
// console if you don't.

#include <Wire.h>
#include "GameCubeController.h"
#include "GameCubeOutput.h"

void boolsFromByte(const unsigned char inputByte, bool bools[8]);

GameCubeController controller;
GameCubeOutput output(2);

void setup()
{
    output.connectController(controller);
    Wire.begin();
}

void loop()
{
    Wire.requestFrom(22, 8);

    while (Wire.available())
    {
        bool buttons1[8];
        boolsFromByte(Wire.read(), buttons1);
        bool buttons2[8];
        boolsFromByte(Wire.read(), buttons2);

        controller.a = buttons1[0];
        controller.b = buttons1[1];
        controller.x = buttons1[2];
        controller.y = buttons1[3];
        controller.z = buttons1[4];
        controller.l = buttons1[5];
        controller.r = buttons1[6];
        controller.start = buttons1[7];

        controller.dLeft = buttons2[0];
        controller.dRight = buttons2[1];
        controller.dDown = buttons2[2];
        controller.dUp = buttons2[3];

        controller.lsX = Wire.read();
        controller.lsY = Wire.read();
        controller.cX = Wire.read();
        controller.cY = Wire.read();
        controller.lAnalog = Wire.read();
        controller.rAnalog = Wire.read();
    }

    output.process();
    controller.endCycle();
}

void boolsFromByte(const unsigned char inputByte, bool bools[8])
{
    for (int index = 0; index < 8; ++index)
        bools[index] = (inputByte & (1 << index)) != 0;
}

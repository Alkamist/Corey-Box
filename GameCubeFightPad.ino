#include "NoMacroController.h"

NoMacroController controller;

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);
}

// This is the main loop that is running every clock cycle
void loop()
{
    /*if (testLeft.getXControl().hasChanged())
    {
        Serial.print("X: ");
        Serial.println(testLeft.getXControl().getValue());
    }

    if (testLeft.getYControl().hasChanged())
    {
        Serial.print("Y: ");
        Serial.println(testLeft.getYControl().getValue());
    }*/

    controller.update();
}
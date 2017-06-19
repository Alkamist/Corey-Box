#include "VirtualGameCubeController.h"
#include "NoMacroController.h"

// Output pin for the gamecube
VirtualGameCubeController controllerOutput(26);

NoMacroController controller;

// This function runs one time when you plug in the controller
void setup()
{
    //Serial.begin(9600);

    controllerOutput.setControls(controller.getControls());
}

// This is the main loop that is running every clock cycle
void loop()
{
    controller.update();
    controllerOutput.update();

    //if (controller.getControls().lsX.hasChanged())
    //    Serial.println(controller.getControls().lsX.getValue());
}

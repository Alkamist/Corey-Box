#include "VirtualGameCubeController.h"
#include "ButtonOnlyController.h"

// Output pin for the gamecube
VirtualGameCubeController controllerOutput(26);

ButtonOnlyController controller;

ButtonReader test(0);

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);
}

// This is the main loop that is running every clock cycle
void loop()
{
    controller.update();
    controllerOutput.update();

    controllerOutput.setControls(controller.getControls());
}

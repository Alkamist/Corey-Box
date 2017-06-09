#include "VirtualGameCubeController.h"
#include "ButtonOnlyController.h"

// Misc values
const unsigned int rExtraHoldTime = 108;
const unsigned int tiltTempDisableTime = 50;

// Output pin for the gamecube
VirtualGameCubeController controllerOutput(26);

ButtonOnlyController controller;

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);
}

// This is the main loop that is running every clock cycle
void loop()
{
    //updateButtons();
    controller.update();
    controllerOutput.update();

    controllerOutput.setControls(controller.getControls());
}
//#include "VirtualGameCubeController.h"
//#include "ButtonOnlyController.h"

#include "DoubleModAxis.h"
#include "ButtonReader.h"
#include "ControlValue.h"

// Misc values
//const unsigned int rExtraHoldTime = 108;
//const unsigned int tiltTempDisableTime = 50;

// Output pin for the gamecube
//VirtualGameCubeController controllerOutput(26);

//ButtonOnlyController controller;

ButtonReader lowButton(0);
ButtonReader highButton(1);
ButtonReader mod1Activator(18);
ButtonReader mod2Activator(24);
ButtonReader tiltButton(7);
ButtonReader tiltTempDisableButton(17);

DoubleModAxis combinedModAxis(lowButton,
                                    highButton,
                                    mod1Activator,
                                    mod2Activator,
                                    tiltButton,
                                    tiltTempDisableButton);

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);
}

// This is the main loop that is running every clock cycle
void loop()
{
    lowButton.update();
    highButton.update();
    mod1Activator.update();
    mod2Activator.update();
    tiltButton.update();
    tiltTempDisableButton.update();

    combinedModAxis.update();

    if (combinedModAxis.hasChanged())
    {
        Serial.println(combinedModAxis.getValue());
    }

    //controller.update();
    //controllerOutput.update();

    //controllerOutput.setControls(controller.getControls());
}
//#include "VirtualGameCubeController.h"
//#include "ButtonOnlyController.h"

#include "WavedashMacro.h"
#include "ButtonReader.h"

// Misc values
//const unsigned int rExtraHoldTime = 108;
//const unsigned int tiltTempDisableTime = 50;

// Output pin for the gamecube
//VirtualGameCubeController controllerOutput(26);

//ButtonOnlyController controller;

ButtonReader lButton(0);
ButtonReader trimUp(17);
ButtonReader trimDown(8);

WavedashMacro wavedashMacro(lButton,
                            trimUp,
                            trimDown);

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);
}

// This is the main loop that is running every clock cycle
void loop()
{
    //controller.update();
    //controllerOutput.update();

    //controllerOutput.setControls(controller.getControls());

    //if (controller.getControls().lsY.hasChanged())
    //    Serial.println(controller.getControls().lsY.getValue());

    lButton.update();
    trimUp.update();
    trimDown.update();
    wavedashMacro.update();

    if (wavedashMacro.getJump().justActivated())
    {
        Serial.print("Jump: ");
        Serial.println(wavedashMacro.getJump().getValue());
    }

    if (wavedashMacro.getAirDodge().justActivated())
    {
        Serial.print("Air Dodge: ");
        Serial.println(wavedashMacro.getAirDodge().getValue());
    }
}

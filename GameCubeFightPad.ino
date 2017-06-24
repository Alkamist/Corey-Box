#include "GameCubeOutput.h"
#include "MacroController.h"

GameCubeOutput gameCubeOutput(26);
MacroController controller;

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);

    gameCubeOutput.connectController(controller);
}

// This is the main loop that is running every clock cycle
void loop()
{
    /*if (controller.lsX.hasChanged())
    {
        Serial.print("X: ");
        Serial.println(controller.lsX.getValue());
    }

    if (controller.lsY.hasChanged())
    {
        Serial.print("Y: ");
        Serial.println(controller.lsY.getValue());
    }*/

    controller.update();
    gameCubeOutput.update();
}

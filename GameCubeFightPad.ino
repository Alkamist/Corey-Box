#include "GameCubeOutput.h"
#include "ButtonOnlyController.h"

GameCubeOutput gameCubeOutput(26);
ButtonOnlyController controller;

// This function runs one time when you plug in the controller
void setup()
{
    //Serial.begin(9600);

    gameCubeOutput.connectController(controller);
}

// This is the main loop that is running every clock cycle
void loop()
{
    controller.process();
    gameCubeOutput.process();

    /*if (controller.lsX.justChanged())
    {
        Serial.print("X: ");
        Serial.println(controller.lsX);
    }

    if (controller.lsY.justChanged())
    {
        Serial.print("Y: ");
        Serial.println(controller.lsY);
    }*/

    controller.endCycle();
}
#include "GameCubeOutput.h"
#include "MacroController.h"

GameCubeOutput gameCubeOutput(26);
MacroController controller;
//elapsedMicros testTimer;

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);

    gameCubeOutput.connectController(controller);
}

// This is the main loop that is running every clock cycle
void loop()
{
    controller.update();

    /*if (controller.getLButton().justActivated())
        testTimer = 0;

    if (controller.l.justActivated())
    {
        Serial.print("L: ");
        Serial.println(testTimer);
    }

    if (controller.r.justActivated())
    {
        Serial.print("R: ");
        Serial.println(testTimer);
    }*/

    gameCubeOutput.update();
}

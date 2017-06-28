#include "GameCubeOutput.h"
#include "MacroController.h"

MacroController controller;
GameCubeOutput gameCubeOutput(26);

//elapsedMicros testTimer;

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

    /*if (controller.getLButton().justActivated())
        testTimer = 0;

    if (controller.l->justActivated())
    {
        Serial.print("L: ");
        Serial.println(testTimer);
    }

    if (controller.r->justActivated())
    {
        Serial.print("R: ");
        Serial.println(testTimer);
    }*/

    controller.endCycle();
}

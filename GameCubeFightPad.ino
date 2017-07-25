#include "GameCubeOutput.h"
#include "ButtonOnlyController.h"

GameCubeOutput gameCubeOutput(45);
ButtonOnlyController controller;

// This function runs one time when you plug in the controller
void setup()
{
    gameCubeOutput.connectController(controller);
}

// This is the main loop that is running every clock cycle
void loop()
{
    controller.process();
    gameCubeOutput.process();

    controller.endCycle();
}

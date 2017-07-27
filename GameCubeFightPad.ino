#include "GameCubeOutput.h"
#include "ButtonOnlyController.h"
//#include "HybridController.h"

GameCubeOutput gameCubeOutput(45);
ButtonOnlyController controller;
//HybridController controller;

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

/*#include "Nintendo.h"

CGamecubeConsole console(45);
CGamecubeController controller(17);

void setup()
{}

void loop()
{
    if (controller.read())
        console.write(controller);
}*/

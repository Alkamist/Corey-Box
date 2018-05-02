#include "GameCubeOutput.h"
#include "EightWayController.h"

GameCubeOutput output(45);
EightWayController controller;

void setup()
{
    output.connectController(controller);
}

void loop()
{
    controller.process();
    output.process();
    controller.endCycle();
}
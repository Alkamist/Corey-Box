#include "GameCubeOutput.h"
#include "ButtonOnlyController.h"

GameCubeOutput output(45);
ButtonOnlyController controller;

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
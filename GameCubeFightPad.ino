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

    controller.endCycle();
}

/*#include "ButtonReader.h"
#include "DoubleModAxis.h"

ButtonReader low(0);
ButtonReader high(1);
ButtonReader mod1(2);
ButtonReader mod2(3);

DoubleModAxis test;

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);
}

// This is the main loop that is running every clock cycle
void loop()
{
    test.setLowState(low);
    test.setHighState(high);
    test.setMod1State(mod1);
    test.setMod2State(mod2);

    test.process();

    if (test.justChanged())
        Serial.println(test);

    test.endCycle();
    low.endCycle();
    high.endCycle();
    mod1.endCycle();
    mod2.endCycle();
}*/
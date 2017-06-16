//#include "VirtualGameCubeController.h"
//#include "ButtonOnlyController.h"

#include "ButtonReader.h"
#include "DoubleModAxis.h"

// Misc values
//const unsigned int rExtraHoldTime = 108;
//const unsigned int tiltTempDisableTime = 50;

// Output pin for the gamecube
//VirtualGameCubeController controllerOutput(26);

//ButtonOnlyController controller;

ButtonReader low(0);
ButtonReader high(1);
ButtonReader mod1(18);
ButtonReader mod2(25);
ButtonReader tilt(17);
ButtonReader tiltTempDisable(11);

DoubleModAxis testAxis(low, high,
                       mod1, mod2,
                       tilt, tiltTempDisable);

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

    low.update();
    high.update();
    mod1.update();
    mod2.update();
    tilt.update();
    tiltTempDisable.update();

    testAxis.update();

    if (testAxis.hasChanged())
        Serial.println(testAxis.getValue());
}

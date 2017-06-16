//#include "VirtualGameCubeController.h"
//#include "ButtonOnlyController.h"

#include "ButtonReader.h"
#include "AnalogAxis.h"
#include "TwoButtonControl.h"
#include "AxisScaler.h"
#include "ValueScaler.h"

// Misc values
//const unsigned int rExtraHoldTime = 108;
//const unsigned int tiltTempDisableTime = 50;

// Output pin for the gamecube
//VirtualGameCubeController controllerOutput(26);

//ButtonOnlyController controller;

ButtonReader low(0);
ButtonReader high(1);
ButtonReader mod(17);

TwoButtonControl twoButtonControl(low, high);
AxisScaler testScaler(0.5, mod);

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
    mod.update();
    twoButtonControl.update();
    testScaler.update();

    testScaler.apply(twoButtonControl);

    if (twoButtonControl.hasChanged())
        Serial.println(twoButtonControl.getValue());
}

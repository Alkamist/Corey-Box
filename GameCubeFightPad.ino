//#include "VirtualGameCubeController.h"
//#include "NoMacroController.h"

#include "ButtonReader.h"
#include "TwoButtonControl.h"
//#include "Joystick.h"

// Output pin for the gamecube
//VirtualGameCubeController controllerOutput(26);

//NoMacroController controller;

/*ButtonReader left(0);
ButtonReader down(1);
ButtonReader up(2);
ButtonReader right(3);

TwoButtonControl xAxis;
TwoButtonControl yAxis;

Joystick testStick;*/

ButtonReader low(0);
ButtonReader high(1);

TwoButtonControl test(low, high);

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);

    //controllerOutput.setControls(controller.getControls());
}

// This is the main loop that is running every clock cycle
void loop()
{
    low.update();
    high.update();

    test.update();

    if (test.hasChanged())
        Serial.println(test.getValue());

    //controller.update();
    //controllerOutput.update();

    /*left.update();
    down.update();
    up.update();
    right.update();

    xAxis.update();
    yAxis.update();

    testStick.update();

    xAxis.setStates(left.isActive(), right.isActive());
    yAxis.setStates(down.isActive(), up.isActive());

    //testStick.setCoordinates(xAxis, yAxis);

    testStick.setXValue(xAxis);
    testStick.setYValue(yAxis);

    if (testStick.hasMoved())
    {
        Serial.print("X: ");
        Serial.println(testStick.getXValue().getValue());
        Serial.print("Y: ");
        Serial.println(testStick.getYValue().getValue());
    }*/

    //if (controller.getControls().lsX.hasChanged())
    //    Serial.println(controller.getControls().lsX.getValue());
}

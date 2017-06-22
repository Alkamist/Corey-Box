//#include "VirtualGameCubeController.h"
//#include "NoMacroController.h"

#include "ButtonReader.h"
#include "ButtonOnlyLeftStick.h"

ButtonReader tiltButton(27);
ButtonReader lsLeftButton(0);
ButtonReader lsDownButton(1);
ButtonReader lsUpButton(2);
ButtonReader lsRightButton(3);
ButtonReader yMod2Button(4);
ButtonReader xMod2Button(5);
ButtonReader yMod1Button(7);
ButtonReader xMod1Button(8);
ButtonReader cLeftButton(9);
ButtonReader cDownButton(10);
ButtonReader cUpButton(11);
ButtonReader cRightButton(12);
ButtonReader bButton(13);
ButtonReader aButton(14);
ButtonReader lButton(15);
ButtonReader jumpButton(16);
ButtonReader zButton(17);
ButtonReader rButton(25);
ButtonReader startButton(24);
ButtonReader dUpButton(23);
ButtonReader dDownButton(22);
ButtonReader dLeftButton(21);
ButtonReader dRightButton(20);
ButtonReader shieldDropButton(19);

ButtonOnlyLeftStick testLeft(lsLeftButton, lsRightButton,
                         lsDownButton, lsUpButton,
                         xMod1Button, xMod2Button,
                         yMod1Button, yMod2Button,
                         tiltButton, lButton,
                         shieldDropButton);

void updateButtons();

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);

    //controllerOutput.setControls(controller.getControls());
}

// This is the main loop that is running every clock cycle
void loop()
{
    updateButtons();
    testLeft.update();

    if (testLeft.getXValue().hasChanged())
    {
        Serial.print("X: ");
        Serial.println(testLeft.getXValue().getValue());
    }

    if (testLeft.getYValue().hasChanged())
    {
        Serial.print("Y: ");
        Serial.println(testLeft.getYValue().getValue());
    }

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

void updateButtons()
{
    tiltButton.update();
    shieldDropButton.update();

    lsLeftButton.update();
    lsRightButton.update();
    lsDownButton.update();
    lsUpButton.update();

    xMod1Button.update();
    xMod2Button.update();
    yMod1Button.update();
    yMod2Button.update();

    cLeftButton.update();
    cRightButton.update();
    cDownButton.update();
    cUpButton.update();

    aButton.update();
    bButton.update();
    jumpButton.update();
    zButton.update();
    rButton.update();
    lButton.update();
    startButton.update();
    dLeftButton.update();
    dRightButton.update();
    dDownButton.update();
    dUpButton.update();
}
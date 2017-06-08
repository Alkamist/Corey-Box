#include "Nintendo.h"
#include "VirtualController.h"
#include "ButtonReader.h"
#include "TwoButtonAxis.h"

// Forward declarations
void updateButtons();

// Misc values
const unsigned int rExtraHoldTime = 108;
const unsigned int tiltTempDisableTime = 50;

// Output pin for the gamecube
CGamecubeConsole gamecubeConsole(25);

// Physical buttons (skip pin 6 because the LED is on it with a Teensy++ 2.0)
ButtonReader tiltModButton(27);
ButtonReader lsLeftButton(0);
ButtonReader lsRightButton(1);
ButtonReader lsDownButton(2);
ButtonReader lsUpButton(3);

ButtonReader xMod1Button(4);
ButtonReader xMod2Button(5);
ButtonReader yMod1Button(7);
ButtonReader yMod2Button(8);

ButtonReader cLeftButton(9);
ButtonReader cRightButton(10);
ButtonReader cDownButton(11);
ButtonReader cUpButton(12);

ButtonReader aButton(13);
ButtonReader bButton(14);
ButtonReader xButton(15);
ButtonReader yButton(16);
ButtonReader zButton(17);
ButtonReader rButton(18);
ButtonReader lButton(19);
ButtonReader startButton(20);
ButtonReader dLeftButton(21);
ButtonReader dRightButton(22);
ButtonReader dDownButton(23);
ButtonReader dUpButton(24);

VirtualController virtualController;

TwoButtonAxis testTwoButtonAxis;

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);

    testTwoButtonAxis.setLowControl(lsLeftButton);
    testTwoButtonAxis.setHighControl(lsRightButton);
}

// This is the main loop that is running every clock cycle
void loop()
{
    updateButtons();
    testTwoButtonAxis.update();

    //uint8_t test = testTwoButtonAxis;

    //if (testTwoButtonAxis.hasChanged())
    //    Serial.println(test);

    virtualController.setLsX(testTwoButtonAxis);

    gamecubeConsole.write(virtualController);
}



void updateButtons()
{
    tiltModButton.update();
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
    xButton.update();
    yButton.update();
    zButton.update();
    rButton.update();
    lButton.update();
    startButton.update();
    dLeftButton.update();
    dRightButton.update();
    dDownButton.update();
    dUpButton.update();
}
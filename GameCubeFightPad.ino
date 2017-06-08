#include "Nintendo.h"
#include "VirtualController.h"
#include "ButtonReader.h"

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

ButtonReader aButtonButton(13);
ButtonReader bButtonButton(14);
ButtonReader xButtonButton(15);
ButtonReader yButtonButton(16);
ButtonReader zButtonButton(17);
ButtonReader rButtonButton(18);
ButtonReader lButtonButton(19);
ButtonReader startButtonButton(20);
ButtonReader dLeftButtonButton(21);
ButtonReader dRightButtonButton(22);
ButtonReader dDownButtonButton(23);
ButtonReader dUpButtonButton(24);

VirtualController virtualController;

// This function runs one time when you plug in the controller
void setup()
{
    Serial.begin(9600);
}

// This is the main loop that is running every clock cycle
void loop()
{
    updateButtons();

    bool test = lsLeftButton;

    if (lsLeftButton.hasChanged())
        Serial.println(test);

    //virtualController.setA();

    //gamecubeConsole.write(virtualController);
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

    aButtonButton.update();
    bButtonButton.update();
    xButtonButton.update();
    yButtonButton.update();
    zButtonButton.update();
    rButtonButton.update();
    lButtonButton.update();
    startButtonButton.update();
    dLeftButtonButton.update();
    dRightButtonButton.update();
    dDownButtonButton.update();
    dUpButtonButton.update();
}
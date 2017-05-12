#include <SPI.h>

#include "DigiPot.h"
#include "TwoButtonController.h"
#include "AxisModSystem.h"
#include "ExtraButtonSystem.h"

ModList meleeModList (0.9500,
                      0.3125,
                      0.4250,
                      0.7000,
                      0.3875,
                      0.1875,
                      0.2125,
                      0.2875,
                      0.6500,
                      104);

ModList projectMModList (0.9500,
                         0.3125,
                         0.5500,
                         0.6750,
                         0.4625,
                         0.2125,
                         0.2250,
                         0.2875,
                         0.6500,
                         104);

// Button pins
const unsigned int lsLeft = 1;
const unsigned int lsRight = 4;
const unsigned int lsDown = 2;
const unsigned int lsUp = 3;

const unsigned int cLeft = 9;
const unsigned int cRight = 14;
const unsigned int cDown = 10;
const unsigned int cUp = 15;

const unsigned int xMod1 = 8;
const unsigned int xMod2 = 6;
const unsigned int yMod1 = 7;
const unsigned int yMod2 = 5;
const unsigned int tiltMod = 0;

// Extra buttons:
const int rButtonSignal = 16;
const int lButtonSignal = 17;
const int rButtonPin = 18;
const int lButtonPin = 19;

// Slave select pins
const unsigned int lsXOutPin = 21;
const unsigned int lsYOutPin = 20;
const unsigned int cXOutPin = 23;
const unsigned int cYOutPin = 22;

// Create digital potentiometer representations
DigiPot lsX (128, lsXOutPin);
DigiPot lsY (128, lsYOutPin);
DigiPot cX (128, cXOutPin);
DigiPot cY (128, cYOutPin);

// Create two button axis controllers
TwoButtonController lsXController (lsLeft, lsRight);
TwoButtonController lsYController (lsDown, lsUp);
TwoButtonController cXController (cLeft, cRight);
TwoButtonController cYController (cDown, cUp);

// Create axis mod system
AxisModSystem axisMods (tiltMod,
                        xMod1,
                        xMod2,
                        yMod1,
                        yMod2);
                        
// Create extra button system
ExtraButtonSystem extraButtonSystem (lButtonPin,
                                     lButtonSignal,
                                     rButtonPin,
                                     rButtonSignal,
                                     tiltMod);
                                     
// This function runs one time when you plug in the controller
void setup()
{
    pinMode (lsLeft, INPUT_PULLUP);
    pinMode (lsRight, INPUT_PULLUP);
    pinMode (lsDown, INPUT_PULLUP);
    pinMode (lsUp, INPUT_PULLUP);
    
    pinMode (cLeft, INPUT_PULLUP);
    pinMode (cRight, INPUT_PULLUP);
    pinMode (cDown, INPUT_PULLUP);
    pinMode (cUp, INPUT_PULLUP);
    
    pinMode (xMod1, INPUT_PULLUP);
    pinMode (xMod2, INPUT_PULLUP);
    pinMode (yMod1, INPUT_PULLUP);
    pinMode (yMod2, INPUT_PULLUP);   
    pinMode (tiltMod, INPUT_PULLUP);

    pinMode (lsXOutPin, OUTPUT);
    pinMode (lsYOutPin, OUTPUT);
    pinMode (cXOutPin, OUTPUT);
    pinMode (cYOutPin, OUTPUT);

    pinMode (rButtonSignal, INPUT);
    pinMode (rButtonPin, INPUT_PULLUP);
    pinMode (lButtonSignal, INPUT);
    pinMode (lButtonPin, INPUT_PULLUP);

    // Set the amount of time in ms R will continue to be held after 
    // being released while holding tilt.
    extraButtonSystem.getRButton().setExtraHoldTime (108);

    // Set the amount of time the tilt modifier will temporarily
    // be disabled after pushing L.
    axisMods.setTiltTempDisableTime (50);

    // Set the potentiometer max ranges to Melee values by default
    lsX.setRange (meleeModList.getMaxiumumValue());
    lsY.setRange (meleeModList.getMaxiumumValue());
    cX.setRange (meleeModList.getMaxiumumValue());
    cY.setRange (meleeModList.getMaxiumumValue());

    // Insert Melee mods into the mod system by default
    axisMods.setModList (meleeModList);

    // Setup for Project M mode
    bool xMod1IsPressed = !digitalRead (xMod1);
    bool yMod2IsPressed = !digitalRead (yMod2);

    // This initiates Project M mode if xMod1 and yMod2
    // are held down when you plug in the controller
    if (xMod1IsPressed && yMod2IsPressed)
    {
        axisMods.setModList (projectMModList);

        lsX.setRange (projectMModList.getMaxiumumValue());
        lsY.setRange (projectMModList.getMaxiumumValue());
        cX.setRange (projectMModList.getMaxiumumValue());
        cY.setRange (projectMModList.getMaxiumumValue());
    }

    SPI.begin();
    Serial.begin (9600);

    // Initialize the pots so they start in the right place.
    // This has to be done after SPI.begin()
    lsX.init();
    lsY.init();
    cX.init();
    cY.init();
}

// This is the main loop that is running every clock cycle
void loop()
{
    extraButtonSystem.update();
  
    lsXController.processCurrentValue();
    lsYController.processCurrentValue();
    cXController.processCurrentValue();
    cYController.processCurrentValue();

    axisMods.setCurrentLsXValue (lsXController.getCurrentValue());
    axisMods.setCurrentLsYValue (lsYController.getCurrentValue());
    axisMods.setCurrentCXValue (cXController.getCurrentValue());
    axisMods.setCurrentCYValue (cYController.getCurrentValue());

    if (lsXController.hasChanged() || lsYController.hasChanged())
        axisMods.resetTiltTimer();

    if (extraButtonSystem.tiltIsTempDisabled())
        axisMods.tempDisableTilt(); 
        
    axisMods.processCurrentValues();

    lsX.setCurrentValue (axisMods.getCurrentLsXValue());
    lsY.setCurrentValue (axisMods.getCurrentLsYValue());
    cX.setCurrentValue (axisMods.getCurrentCXValue());
    cY.setCurrentValue (axisMods.getCurrentCYValue());
    
    lsX.endLoop();
    lsY.endLoop();
    cX.endLoop();
    cY.endLoop();
}

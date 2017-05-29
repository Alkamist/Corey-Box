  #include <SPI.h>

#include "Globals.h"
#include "DigiPot.h"
#include "TwoButtonController.h"
#include "AxisModSystem.h"
#include "ExtraButtonSystem.h"

ModList meleeModList(0.9500,
                     0.3125,
                     0.4250,
                     0.7000,
                     0.3875,
                     0.1875,
                     0.2125,
                     0.2875,
                     0.6500,
                     104);

ModList projectMModList(0.9500,
                        0.3125,
                        0.5500,
                        0.6750,
                        0.4625,
                        0.2125,
                        0.2250,
                        0.2875,
                        0.6500,
                        104);

const unsigned int R_EXTRA_HOLD_TIME = 108;
const unsigned int TILT_TEMP_DISABLE_TIME = 50;

// Button pins
const unsigned int LS_LEFT_PIN = 1;
const unsigned int LS_RIGHT_PIN = 4;
const unsigned int LS_DOWN_PIN = 2;
const unsigned int LS_UP_PIN = 3;

const unsigned int C_LEFT_PIN = 9;
const unsigned int C_RIGHT_PIN = 14;
const unsigned int C_DOWN_PIN = 10;
const unsigned int C_UP_PIN = 15;

const unsigned int X_MOD1_PIN = 8;
const unsigned int X_MOD2_PIN = 6;
const unsigned int Y_MOD1_PIN = 7;
const unsigned int Y_MOD2_PIN = 5;
const unsigned int TILT_MOD_PIN = 0;

// Extra buttons:
const unsigned int R_BUTTON_SIGNAL_PIN = 16;
const unsigned int L_BUTTON_SIGNAL_PIN = 17;
const unsigned int R_BUTTON_PIN = 18;
const unsigned int L_BUTTON_PIN = 19;

// Slave select pins
const unsigned int LSX_OUT_PIN = 21;
const unsigned int LSY_OUT_PIN = 20;
const unsigned int CX_OUT_PIN = 23;
const unsigned int CY_OUT_PIN = 22;

// Create digital potentiometer representations
DigiPot lsX(128, LSX_OUT_PIN);
DigiPot lsY(128, LSY_OUT_PIN);
DigiPot cX(128, CX_OUT_PIN);
DigiPot cY(128, CY_OUT_PIN);

// Create two button axis controllers
TwoButtonController lsXController(LS_LEFT_PIN, LS_RIGHT_PIN);
TwoButtonController lsYController(LS_DOWN_PIN, LS_UP_PIN);
TwoButtonController cXController(C_LEFT_PIN, C_RIGHT_PIN);
TwoButtonController cYController(C_DOWN_PIN, C_UP_PIN);

// Create axis mod system
AxisModSystem axisMods;

// Create extra button system
ExtraButtonSystem extraButtonSystem;

// This function runs one time when you plug in the controller
void setup()
{
    pinMode(LS_LEFT_PIN, INPUT_PULLUP);
    pinMode(LS_RIGHT_PIN, INPUT_PULLUP);
    pinMode(LS_DOWN_PIN, INPUT_PULLUP);
    pinMode(LS_UP_PIN, INPUT_PULLUP);

    pinMode(C_LEFT_PIN, INPUT_PULLUP);
    pinMode(C_RIGHT_PIN, INPUT_PULLUP);
    pinMode(C_DOWN_PIN, INPUT_PULLUP);
    pinMode(C_UP_PIN, INPUT_PULLUP);

    pinMode(X_MOD1_PIN, INPUT_PULLUP);
    pinMode(X_MOD2_PIN, INPUT_PULLUP);
    pinMode(Y_MOD1_PIN, INPUT_PULLUP);
    pinMode(Y_MOD2_PIN, INPUT_PULLUP);
    pinMode(TILT_MOD_PIN, INPUT_PULLUP);

    pinMode(LSX_OUT_PIN, OUTPUT);
    pinMode(LSY_OUT_PIN, OUTPUT);
    pinMode(CX_OUT_PIN, OUTPUT);
    pinMode(CY_OUT_PIN, OUTPUT);

    pinMode(R_BUTTON_SIGNAL_PIN, INPUT);
    pinMode(R_BUTTON_PIN, INPUT_PULLUP);
    pinMode(L_BUTTON_SIGNAL_PIN, INPUT);
    pinMode(L_BUTTON_PIN, INPUT_PULLUP);

    // Set the amount of time in ms R will continue to be held after
    // being released while holding tilt.
    extraButtonSystem.getRButton().setExtraHoldTime(R_EXTRA_HOLD_TIME);

    // Set the amount of time the tilt modifier will temporarily
    // be disabled after pushing L.
    axisMods.setTiltTempDisableTime(TILT_TEMP_DISABLE_TIME);

    // Set the potentiometer max ranges to Melee values by default
    lsX.setRange(meleeModList.getMaxiumumValue());
    lsY.setRange(meleeModList.getMaxiumumValue());
    cX.setRange(meleeModList.getMaxiumumValue());
    cY.setRange(meleeModList.getMaxiumumValue());

    // Insert Melee mods into the mod system by default
    axisMods.setModList(meleeModList);

    // Setup for Project M mode
    bool xMod1IsPressed = !digitalRead(X_MOD1_PIN);
    bool yMod2IsPressed = !digitalRead(Y_MOD2_PIN);

    // This initiates Project M mode if X_MOD1_PIN and Y_MOD2_PIN
    // are held down when you plug in the controller
    if (xMod1IsPressed && yMod2IsPressed)
    {
        axisMods.setModList (projectMModList);

        lsX.setRange(projectMModList.getMaxiumumValue());
        lsY.setRange(projectMModList.getMaxiumumValue());
        cX.setRange(projectMModList.getMaxiumumValue());
        cY.setRange(projectMModList.getMaxiumumValue());
    }

    SPI.begin();

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

    axisMods.setCurrentLsXValue(lsXController.getCurrentValue());
    axisMods.setCurrentLsYValue(lsYController.getCurrentValue());
    axisMods.setCurrentCXValue(cXController.getCurrentValue());
    axisMods.setCurrentCYValue(cYController.getCurrentValue());

    if (lsXController.hasChanged() || lsYController.hasChanged())
        axisMods.resetTiltTimer();

    if (extraButtonSystem.tiltIsTempDisabled())
        axisMods.tempDisableTilt();

    axisMods.processCurrentValues();

    lsX.setCurrentValue(axisMods.getCurrentLsXValue());
    lsY.setCurrentValue(axisMods.getCurrentLsYValue());
    cX.setCurrentValue(axisMods.getCurrentCXValue());
    cY.setCurrentValue(axisMods.getCurrentCYValue());

    lsX.endLoop();
    lsY.endLoop();
    cX.endLoop();
    cY.endLoop();
}

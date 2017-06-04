#include <SPI.h>
#include <Bounce2.h>

#include "Globals.h"
#include "DigiPot.h"
#include "TwoButtonController.h"
#include "AxisModSystem.h"
#include "ExtraButtonSystem.h"

// Forward declarations
void setupButtons();
void updateButtons();

// Mod lists
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

// Extra buttons
const unsigned int R_BUTTON_SIGNAL_PIN = 16;
const unsigned int L_BUTTON_SIGNAL_PIN = 17;
const unsigned int R_BUTTON_PIN = 18;
const unsigned int L_BUTTON_PIN = 19;

// Bounce objects
Bounce LS_LEFT_BOUNCE = Bounce();
Bounce LS_RIGHT_BOUNCE = Bounce();
Bounce LS_DOWN_BOUNCE = Bounce();
Bounce LS_UP_BOUNCE = Bounce();

Bounce C_LEFT_BOUNCE = Bounce();
Bounce C_RIGHT_BOUNCE = Bounce();
Bounce C_DOWN_BOUNCE = Bounce();
Bounce C_UP_BOUNCE = Bounce();

Bounce X_MOD1_BOUNCE = Bounce();
Bounce X_MOD2_BOUNCE = Bounce();
Bounce Y_MOD1_BOUNCE = Bounce();
Bounce Y_MOD2_BOUNCE = Bounce();
Bounce TILT_BOUNCE = Bounce();

Bounce L_BUTTON_BOUNCE = Bounce();
Bounce R_BUTTON_BOUNCE = Bounce();

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
    setupButtons();

    pinMode(LSX_OUT_PIN, OUTPUT);
    pinMode(LSY_OUT_PIN, OUTPUT);
    pinMode(CX_OUT_PIN, OUTPUT);
    pinMode(CY_OUT_PIN, OUTPUT);

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
    updateButtons();

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





void setupButtons()
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

    pinMode(R_BUTTON_PIN, INPUT_PULLUP);
    pinMode(L_BUTTON_PIN, INPUT_PULLUP);

    pinMode(L_BUTTON_SIGNAL_PIN, INPUT);
    pinMode(R_BUTTON_SIGNAL_PIN, INPUT);

    // Bounces

    int bounceInterval = 5;

    LS_LEFT_BOUNCE.attach(LS_LEFT_PIN);
    LS_RIGHT_BOUNCE.attach(LS_RIGHT_PIN);
    LS_DOWN_BOUNCE.attach(LS_DOWN_PIN);
    LS_UP_BOUNCE.attach(LS_UP_PIN);

    C_LEFT_BOUNCE.attach(C_LEFT_PIN);
    C_RIGHT_BOUNCE.attach(C_RIGHT_PIN);
    C_DOWN_BOUNCE.attach(C_DOWN_PIN);
    C_UP_BOUNCE.attach(C_UP_PIN);

    X_MOD1_BOUNCE.attach(X_MOD1_PIN);
    X_MOD2_BOUNCE.attach(X_MOD2_PIN);
    Y_MOD1_BOUNCE.attach(Y_MOD1_PIN);
    Y_MOD2_BOUNCE.attach(Y_MOD2_PIN);
    TILT_BOUNCE.attach(TILT_MOD_PIN);

    L_BUTTON_BOUNCE.attach(L_BUTTON_PIN);
    R_BUTTON_BOUNCE.attach(R_BUTTON_PIN);



    LS_LEFT_BOUNCE.interval(bounceInterval);
    LS_RIGHT_BOUNCE.interval(bounceInterval);
    LS_DOWN_BOUNCE.interval(bounceInterval);
    LS_UP_BOUNCE.interval(bounceInterval);

    C_LEFT_BOUNCE.interval(bounceInterval);
    C_RIGHT_BOUNCE.interval(bounceInterval);
    C_DOWN_BOUNCE.interval(bounceInterval);
    C_UP_BOUNCE.interval(bounceInterval);

    X_MOD1_BOUNCE.interval(bounceInterval);
    X_MOD2_BOUNCE.interval(bounceInterval);
    Y_MOD1_BOUNCE.interval(bounceInterval);
    Y_MOD2_BOUNCE.interval(bounceInterval);
    TILT_BOUNCE.interval(bounceInterval);

    L_BUTTON_BOUNCE.interval(bounceInterval);
    R_BUTTON_BOUNCE.interval(bounceInterval);
}

void updateButtons()
{
    LS_LEFT_BOUNCE.update();
    LS_RIGHT_BOUNCE.update();
    LS_DOWN_BOUNCE.update();
    LS_UP_BOUNCE.update();

    C_LEFT_BOUNCE.update();
    C_RIGHT_BOUNCE.update();
    C_DOWN_BOUNCE.update();
    C_UP_BOUNCE.update();

    X_MOD1_BOUNCE.update();
    X_MOD2_BOUNCE.update();
    Y_MOD1_BOUNCE.update();
    Y_MOD2_BOUNCE.update();
    TILT_BOUNCE.update();

    L_BUTTON_BOUNCE.update();
    R_BUTTON_BOUNCE.update();
}
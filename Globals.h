#ifndef GLOBALS_H
#define GLOBALS_H

// Button pins
extern const unsigned int LS_LEFT_PIN;
extern const unsigned int LS_RIGHT_PIN;
extern const unsigned int LS_DOWN_PIN;
extern const unsigned int LS_UP_PIN;

extern const unsigned int C_LEFT_PIN;
extern const unsigned int C_RIGHT_PIN;
extern const unsigned int C_DOWN_PIN;
extern const unsigned int C_UP_PIN;

extern const unsigned int X_MOD1_PIN;
extern const unsigned int X_MOD2_PIN;
extern const unsigned int Y_MOD1_PIN;
extern const unsigned int Y_MOD2_PIN;
extern const unsigned int TILT_MOD_PIN;

// Extra buttons:
extern const unsigned int R_BUTTON_SIGNAL_PIN;
extern const unsigned int L_BUTTON_SIGNAL_PIN;
extern const unsigned int R_BUTTON_PIN;
extern const unsigned int L_BUTTON_PIN;

// Slave select pins
extern const unsigned int LSX_OUT_PIN;
extern const unsigned int LSY_OUT_PIN;
extern const unsigned int CX_OUT_PIN;
extern const unsigned int CY_OUT_PIN;

extern const unsigned int R_EXTRA_HOLD_TIME;
extern const unsigned int TILT_TEMP_DISABLE_TIME;

// Bounce objects
extern Bounce LS_LEFT_BOUNCE;
extern Bounce LS_RIGHT_BOUNCE;
extern Bounce LS_DOWN_BOUNCE;
extern Bounce LS_UP_BOUNCE;

extern Bounce C_LEFT_BOUNCE;
extern Bounce C_RIGHT_BOUNCE;
extern Bounce C_DOWN_BOUNCE;
extern Bounce C_UP_BOUNCE;

extern Bounce X_MOD1_BOUNCE;
extern Bounce X_MOD2_BOUNCE;
extern Bounce Y_MOD1_BOUNCE;
extern Bounce Y_MOD2_BOUNCE;
extern Bounce TILT_BOUNCE;

extern Bounce L_BUTTON_BOUNCE;
extern Bounce R_BUTTON_BOUNCE;

#endif // EXTRABUTTONSYSTEM_H
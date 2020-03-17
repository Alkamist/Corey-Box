/*
  DIYB0XX Native USB v1.201 code by Crane.
  This code utilizes
    Nicohood's Nintendo library
    MHeironimus' Arduino Joystick Library.
  This is code designed with native USB 8 bit AVR microcontrollers in mind.
  I test these on my 32u4 boards (Like the Arduino Micro).
  If you use it on any other device, do so at your own risk.
  A version of this code is available without native USB joystick support DIY controllers
  using other Arduinos here https://github.com/Crane1195/DIYB0XX/tree/master/code

  Read the README file for whichever of these you are using for more information.
*/

bool UseNewModVertical = true;

uint8_t fTwoIPNoReactivate(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH, bool& lockLOW, bool& lockHIGH);
uint8_t fTwoIP(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH);
uint8_t fNeutral(bool isLOW, bool isHIGH);

enum SOCD
{
  Neutral,
  TwoIP,
  TwoIPNoReactivate
};

bool wasLEFT = false;
bool wasRIGHT = false;
bool wasUP = false;
bool wasDOWN = false;

bool wasCLEFT = false;
bool wasCRIGHT = false;
bool wasCUP = false;
bool wasCDOWN = false;

bool lockLEFT = false;
bool lockRIGHT = false;
bool lockUP = false;
bool lockDOWN = false;

bool lockCLEFT = false;
bool lockCRIGHT = false;
bool lockCUP = false;
bool lockCDOWN = false;

SOCD currentSOCD = TwoIPNoReactivate;

const int L = 2;
const int LEFT = 3;
const int DOWN = 4;
const int RIGHT = 5;
const int MOD1 = 7;
const int MOD2 = 8;

const int START = 9;
const int B = 16;
const int X = 23;
const int Z = 22;
const int UP = 19;
const int R = 20;
const int Y = 21;

const int CDOWN = 10;
const int A = 15;
const int CRIGHT = 17;
const int CLEFT = 14;
const int CUP = 18;

const uint8_t minValue = 28;
const uint8_t maxValue = 228;

void setup()
{
  Joystick.useManualSend(true);
  pinMode(L, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(MOD1, INPUT_PULLUP);
  pinMode(MOD2, INPUT_PULLUP);
  pinMode(START, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  pinMode(X, INPUT_PULLUP);
  pinMode(Z, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(R, INPUT_PULLUP);
  pinMode(Y, INPUT_PULLUP);
  pinMode(CDOWN, INPUT_PULLUP);
  pinMode(A, INPUT_PULLUP);
  pinMode(CRIGHT, INPUT_PULLUP);
  pinMode(CLEFT, INPUT_PULLUP);
  pinMode(CUP, INPUT_PULLUP);
}

void loop()
{
  bool isL = (digitalRead(L) == LOW);
  bool isLEFT = (digitalRead(LEFT) == LOW);
  bool isDOWN = (digitalRead(DOWN) == LOW);
  bool isRIGHT = (digitalRead(RIGHT) == LOW);
  bool isMOD1 = (digitalRead(MOD1) == LOW);
  bool isMOD2 = (digitalRead(MOD2) == LOW);
  bool isSTART = (digitalRead(START) == LOW);
  bool isB = (digitalRead(B) == LOW);
  bool isX = (digitalRead(X) == LOW);
  bool isZ = (digitalRead(Z) == LOW);
  bool isUP = (digitalRead(UP) == LOW);
  bool isR = (digitalRead(R) == LOW);
  bool isY = (digitalRead(Y) == LOW);
  bool isCDOWN = (digitalRead(CDOWN) == LOW);
  bool isA = (digitalRead(A) == LOW);
  bool isCRIGHT = (digitalRead(CRIGHT) == LOW);
  bool isCLEFT = (digitalRead(CLEFT) == LOW);
  bool isCUP = (digitalRead(CUP) == LOW);

  bool isDPADUP = false;
  bool isDPADDOWN = false;
  bool isDPADLEFT = false;
  bool isDPADRIGHT = false;

  uint8_t controlX = 128;
  uint8_t controlY = 128;
  uint8_t cstickX = 128;
  uint8_t cstickY = 128;

  int8_t positionX = 0;
  int8_t positionY = 0;
  int8_t positionCX = 0;
  int8_t positionCY = 0;

  bool HORIZONTAL = false;
  bool VERTICAL = false;
  bool DIAGONAL = false;

  if (currentSOCD == TwoIPNoReactivate) {
    controlX = fTwoIPNoReactivate(isLEFT, isRIGHT, wasLEFT, wasRIGHT, lockLEFT, lockRIGHT);
    controlY = fTwoIPNoReactivate(isDOWN, isUP, wasDOWN, wasUP, lockDOWN, lockUP);
    cstickX = fTwoIPNoReactivate(isCLEFT, isCRIGHT, wasCLEFT, wasCRIGHT, lockCLEFT, lockCRIGHT);
    cstickY = fTwoIPNoReactivate(isCDOWN, isCUP, wasCDOWN, wasCUP, lockCDOWN, lockCUP);
  }

  if (currentSOCD == TwoIP){
    controlX = fTwoIP(isLEFT, isRIGHT, wasLEFT, wasRIGHT);
    controlY = fTwoIP(isDOWN, isUP, wasDOWN, wasUP);
    cstickX = fTwoIP(isCLEFT, isCRIGHT, wasCLEFT, wasCRIGHT);
    cstickY = fTwoIP(isCDOWN, isCUP, wasCDOWN, wasCUP);
  }

  if (currentSOCD == Neutral) {
    controlX = fNeutral(isLEFT, isRIGHT);
    controlY = fNeutral(isDOWN, isUP);
    cstickX = fNeutral(isCLEFT, isCRIGHT);
    cstickY = fNeutral(isCDOWN, isCUP);
  }

  if (controlX != 128) {
    HORIZONTAL = true;
    if (controlX == minValue) positionX = -1;
    else positionX = 1;
  }
  if (controlY != 128) {
    VERTICAL = true;
    if (controlY == minValue) positionY = -1;
    else positionY = 1;
  }
  if (HORIZONTAL && VERTICAL) DIAGONAL = true;

  if (cstickX != 128) {
    if (cstickX == minValue) positionCX = -1;
    else positionCX = 1;
  }
  if (cstickY != 128) {
    if (cstickY == minValue) positionCY = -1;
    else positionCY = 1;
  }

  if (isMOD1) {
    if (HORIZONTAL) {
      controlX = 128 + (positionX * 53);
    }
    if (VERTICAL) {
      if (UseNewModVertical)
        controlY = 128 + (positionY * 23);
      else
        controlY = 128 + (positionY * 52);
    }

    if (isB) {
      controlX = 128 + (positionX * 53);
    }
    if (positionCX != 0){
      cstickX = 128 + (positionCX * 65);
      cstickY = 128 + (positionY * 23);
    }

    if (DIAGONAL) {
      controlX = 128 + (positionX * 59);
      controlY = 128 + (positionY * 23);

      if (isCUP) {
        controlX = 128 + (positionX * 49);
        controlY = 128 + (positionY * 35);
      }

      if (isCDOWN) {
        controlX = 128 + (positionX * 49);
        controlY = 128 + (positionY * 24);
      }

      if (isCLEFT) {
        controlX = 128 + (positionX * 52);
        controlY = 128 + (positionY * 31);
      }

      if (isCRIGHT) {
        controlX = 128 + (positionX * 51);
        controlY = 128 + (positionY * 43);
      }
    }
  }

  if (isMOD2) {
    if (HORIZONTAL) {
      controlX = 128 + (positionX * 27);
    }
    if (VERTICAL) {
      controlY = 128 + (positionY * 59);
    }

    if (isB) {
      controlX = 128 + (positionX * 80);
    }

    if (DIAGONAL) {
      controlX = 128 + (positionX * 23);
      controlY = 128 + (positionY * 59);

      if (isCUP) {
        controlX = 128 + (positionX * 45);
        controlY = 128 + (positionY * 63);
      }

      if (isCDOWN) {
        controlX = 128 + (positionX * 28);
        controlY = 128 + (positionY * 57);
      }

      if (isCLEFT) {
        controlX = 128 + (positionX * 34);
        controlY = 128 + (positionY * 57);
      }

      if (isCRIGHT) {
        controlX = 128 + (positionX * 47);
        controlY = 128 + (positionY * 57);
      }
    }
  }

  if (isLEFT && isRIGHT && !VERTICAL)
    controlX = 128 + (positionX * 100);

  if (isL) {
    if (HORIZONTAL) controlX = 128 + (positionX * 100);
    if (VERTICAL) controlY = 128 + (positionY * 100);
    if (HORIZONTAL && (positionY == 1)) {
      controlX = 128 + (positionX * 43);
      controlY = 128 + 43;
    }
    if (HORIZONTAL && (positionY == -1)) {
      controlX = 128 + (positionX * 57);
      controlY = 128 - 55;
    }
    if ((isMOD1 || isMOD2)) {
      if (DIAGONAL) {
        if (isMOD1) {
          controlX = 128 + (positionX * 51);
          controlY = 128 + (positionY * 30);
        }
        if (isMOD2) {
          controlX = 128 + (positionX * 40);
          controlY = 128 + (positionY * 68);
        }
      }
    }
  }

  if (isR) {
    if (HORIZONTAL) {
      controlX = 128 + (positionX * 51);
    }
    if (VERTICAL) {
      controlY = 128 + (positionY * 43);
    }
    if (DIAGONAL) {
      controlX = 128 + (positionX * 43);
      if (isMOD1) {
        controlX = 128 + (positionX * 51);
        controlY = 128 + (positionY * 30);
      }
      if (isMOD2) {
        controlX = 128 + (positionX * 40);
        controlY = 128 + (positionY * 68);
      }
    }
  }

  if (isMOD1 && isMOD2) {
    cstickX = 128;
    cstickY = 128;
    if (isCUP) isDPADUP = true;
    if (isCDOWN) isDPADDOWN = true;
    if (isCLEFT) isDPADLEFT = true;
    if (isCRIGHT) isDPADRIGHT = true;
  }

  Joystick.button(1, isX);
  Joystick.button(2, isA);
  Joystick.button(3, isB);
  Joystick.button(4, isY);
  Joystick.button(5, isL);
  Joystick.button(6, isR);
  Joystick.button(8, isZ);
  Joystick.button(10, isSTART);
  Joystick.button(13, isDPADUP);
  Joystick.button(15, isDPADDOWN);
  Joystick.button(16, isDPADLEFT);
  Joystick.button(14, isDPADRIGHT);

  float lsXOut = (controlX - 128) / 80.0;
  float lsYOut = (controlY - 128) / 80.0;
  float cXOut = (cstickX - 128) / 80.0;
  float cYOut = (cstickY - 128) / 80.0;

  if (lsXOut >= 0.0)
    Joystick.X(lsXOut * 322 + 512);
  else
    Joystick.X(lsXOut * 320 + 512);
  if (lsYOut >= 0.0)
    Joystick.Y(-lsYOut * 328 + 512);
  else
    Joystick.Y(-lsYOut * 318 + 512);
  if (cYOut >= 0.0)
    Joystick.Z(cYOut * 322 + 512);
  else
    Joystick.Z(cYOut * 320 + 512);
  if (cXOut >= 0.0)
    Joystick.Zrotate(-cXOut * 298 + 680);
  else
    Joystick.Zrotate(-cXOut * 220 + 680);

  Joystick.send_now();
}

uint8_t fTwoIPNoReactivate(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH, bool& lockLOW, bool& lockHIGH){
  uint8_t control = 128;
  if (isLOW && isHIGH) {
    if (wasHIGH) {
      control = minValue;
      lockHIGH = true; }
    if (wasLOW) {
      control = maxValue;
      lockLOW = true; }
  }
  if (!isLOW && isHIGH && (lockHIGH == false)) {
    control = maxValue;
    wasHIGH = true;
    wasLOW = false;
    lockLOW = false;
  }
  if (isLOW && !isHIGH && (lockLOW == false)) {
    control = minValue;
    wasLOW = true;
    wasHIGH = false;
    lockHIGH = false;
  }
  if (!isLOW && !isHIGH) {
    wasHIGH = false;
    wasLOW = false;
    lockLOW = false;
    lockHIGH = false;
  }
  return control;
}

uint8_t fTwoIP(bool isLOW, bool isHIGH, bool& wasLOW, bool& wasHIGH){
  uint8_t control = 128;
  if (isLOW && wasHIGH)
    control = minValue;
  if (isHIGH && wasLOW)
    control = maxValue;
  if (!isLOW && isHIGH) {
    control = maxValue;
    wasHIGH = true;
    wasLOW = false; }
  if (isLOW && !isHIGH) {
    control = minValue;
    wasLOW = true;
    wasHIGH = false;
  }
  return control;
}

uint8_t fNeutral(bool isLOW, bool isHIGH){
  uint8_t control = 128;
  if (!isLOW && isHIGH)
    control = maxValue;
  if (isLOW && !isHIGH)
    control = minValue;
  return control;
}

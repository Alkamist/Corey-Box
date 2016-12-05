#include <SPI.h>

// Forward declarations:
void writeToDigitalPot (int writePin, int inputValue);
int getPotValue (int potLowPin, int potHighPin, int &potPressOrder);
void applyModToPot (int &inputPotValue, double inputModDecimal);
void applyLeftStickModifiers (int &xAxisValue, int &yAxisValue);

// Setup potentiometer values:
const int masterPotResolution = 128;
const int masterPotMiddle = ceil (masterPotResolution * 0.5);
const int masterPotMinimum = 6;
const int masterPotMaximum = 122;

const byte potWriteAddress = 0x00;

// Slave Select Pins:
const int lsXOutPin = 20;
const int lsYOutPin = 21;
const int cXOutPin = 23;
const int cYOutPin = 22;

// Buttons:
const int lsLeft = 0;
const int lsRight = 1;
const int lsDown = 2;
const int lsUp = 3;
const int xMod1 = 4;
const int xMod2 = 5;
const int yMod1 = 6;
const int yMod2 = 7;
const int cLeft = 8;
const int cRight = 9;
const int cDown = 10;
const int cUp = 17;
const int tiltMod = 15;

// Potentiometer low/high press-order states:
// 0 means low was pressed first.
// 1 means high was pressed first.
// -1 means neither was pressed first.
int lsXOrder = -1;
int lsYOrder = -1;
int cXOrder = -1;
int cYOrder = -1;

// Axis values in the previous cycle:
int lsXLastValue = masterPotMiddle;
int lsYLastValue = masterPotMiddle;
int cXLastValue = masterPotMiddle;
int cYLastValue = masterPotMiddle;

// Modifier values:
const double mod1Decimal = 0.31;
const double mod2Decimal = 0.41;
const double combinedMod1TiltDecimal = 0.65;
const double combinedMod2TiltDecimal = 0.85;
const double combinedModDecimal = 0.65;
const double tiltDecimal = 0.39;

// C-Stick Y axis skew for angled smashes:
double cYAxisSkew = 0.65;

void setup()
{
    pinMode (lsLeft, INPUT_PULLUP);
    pinMode (lsRight, INPUT_PULLUP);
    pinMode (lsDown, INPUT_PULLUP);
    pinMode (lsUp, INPUT_PULLUP);
    pinMode (xMod1, INPUT_PULLUP);
    pinMode (xMod2, INPUT_PULLUP);
    pinMode (yMod1, INPUT_PULLUP);
    pinMode (yMod2, INPUT_PULLUP);
    pinMode (cLeft, INPUT_PULLUP);
    pinMode (cRight, INPUT_PULLUP);
    pinMode (cDown, INPUT_PULLUP);
    pinMode (cUp, INPUT_PULLUP);
    pinMode (tiltMod, INPUT_PULLUP);

    pinMode (lsXOutPin, OUTPUT);
    pinMode (lsYOutPin, OUTPUT);
    pinMode (cXOutPin, OUTPUT);
    pinMode (cYOutPin, OUTPUT);

    SPI.begin();

    writeToDigitalPot (lsXOutPin, masterPotMiddle);
    writeToDigitalPot (lsYOutPin, masterPotMiddle);
    writeToDigitalPot (cXOutPin, masterPotMiddle);
    writeToDigitalPot (cYOutPin, masterPotMiddle);
}

void loop()
{
    //-------------Left Stick--------------
    int lsXValue = getPotValue (lsLeft, lsRight, lsXOrder);
    int lsYValue = getPotValue (lsDown, lsUp, lsYOrder);

    applyLeftStickModifiers (lsXValue, lsYValue);

    if (lsXValue != lsXLastValue)
    {
        writeToDigitalPot (lsXOutPin, lsXValue);
    }
    if (lsYValue != lsYLastValue)
    {
        writeToDigitalPot (lsYOutPin, lsYValue);
    }

    lsXLastValue = lsXValue;
    lsYLastValue = lsYValue;

    //-------------C Stick--------------
    int cXValue = getPotValue (cLeft, cRight, cXOrder);
    int cYValue = getPotValue (cDown, cUp, cYOrder);

    applyModToPot (cYValue, cYAxisSkew);
    
    if (cXValue != cXLastValue)
    {
        writeToDigitalPot (cXOutPin, cXValue);
    }
    if (cYValue != cYLastValue)
    {
        writeToDigitalPot (cYOutPin, cYValue);
    }

    cYLastValue = cYValue;
    cXLastValue = cXValue;
}

void writeToDigitalPot (int writePin, int inputValue)
{
    digitalWrite (writePin, LOW);

    SPI.transfer (potWriteAddress);
    SPI.transfer (inputValue);

    digitalWrite (writePin, HIGH);
}

int getPotValue (int potLowPin, int potHighPin, int &potPressOrder)
{
    bool lowIsPressed = !digitalRead (potLowPin);
    bool highIsPressed = !digitalRead (potHighPin);
    bool lowWasPressedFirst = lowIsPressed && (potPressOrder == 0);
    bool highWasPressedFirst = highIsPressed && (potPressOrder == 1);

    // Low Cases
    if (lowIsPressed && highWasPressedFirst)
    {
        potPressOrder = 1;
        return masterPotMinimum;
    }
    if (lowIsPressed && !highIsPressed)
    {
        potPressOrder = 0;
        return masterPotMinimum;
    }

    // High Cases
    if (lowWasPressedFirst && highIsPressed)
    {
        potPressOrder = 0;
        return masterPotMaximum;
    }
    if (!lowIsPressed && highIsPressed)
    {
        potPressOrder = 1;
        return masterPotMaximum;
    }

    // Middle Cases
    if (!lowIsPressed && !highIsPressed)
    {
        potPressOrder = -1;
        return masterPotMiddle;
    }
    if (lowIsPressed && highIsPressed && (potPressOrder == -1))
    {
        potPressOrder = -1;
        return masterPotMiddle;
    }

    return masterPotMiddle;
}

void applyModToPot (int &inputPotValue, double inputModDecimal)
{
    if (inputPotValue < masterPotMiddle)
    {
        inputPotValue = masterPotMiddle - masterPotMiddle * inputModDecimal + 0.5;
        return;
    }
    if (inputPotValue > masterPotMiddle)
    {
        inputPotValue = masterPotMiddle + masterPotMiddle * inputModDecimal + 0.5;
        return;
    }
}

void applyLeftStickModifiers (int &xAxisValue, int &yAxisValue)
{
    bool xMod1IsPressed = !digitalRead (xMod1);
    bool xMod2IsPressed = !digitalRead (xMod2);
    bool yMod1IsPressed = !digitalRead (yMod1);
    bool yMod2IsPressed = !digitalRead (yMod2);
    bool tiltModIsPressed = !digitalRead (tiltMod);

    // Tilt mod not pressed:
    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod1Decimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod2Decimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (yAxisValue, mod1Decimal);
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod1Decimal);
        applyModToPot (yAxisValue, mod1Decimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod2Decimal);
        applyModToPot (yAxisValue, mod1Decimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        applyModToPot (yAxisValue, mod1Decimal);
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (yAxisValue, mod2Decimal);
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod1Decimal);
        applyModToPot (yAxisValue, mod2Decimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod2Decimal);
        applyModToPot (yAxisValue, mod2Decimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        applyModToPot (yAxisValue, mod2Decimal);
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod1Decimal);
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod2Decimal);
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }

    // Tilt mod pressed:

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, tiltDecimal);
        applyModToPot (yAxisValue, tiltDecimal);
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod1TiltDecimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod2TiltDecimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (yAxisValue, combinedMod1TiltDecimal);
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod1TiltDecimal);
        applyModToPot (yAxisValue, combinedMod1TiltDecimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod2TiltDecimal);
        applyModToPot (yAxisValue, combinedMod1TiltDecimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        applyModToPot (yAxisValue, combinedMod1TiltDecimal);
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (yAxisValue, combinedMod2TiltDecimal);
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod1TiltDecimal);
        applyModToPot (yAxisValue, combinedMod2TiltDecimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod2TiltDecimal);
        applyModToPot (yAxisValue, combinedMod2TiltDecimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        applyModToPot (yAxisValue, combinedMod2TiltDecimal);
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod1TiltDecimal);
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedMod2TiltDecimal);
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        applyModToPot (yAxisValue, combinedModDecimal);
        return;
    }
}

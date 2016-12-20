#include <SPI.h>

// Forward declarations:
void writeToDigitalPot (int writePin, int inputValue);
int getPotValue (int potLowPin, int potHighPin, int &potPressOrder);
void applyModToPot (int &inputPotValue, double inputModDecimal);
void applyCStickModifiers (int cXAxisValue, int &cYAxisValue, int lsYAxisValue);
void applyLeftStickModifiers (int &xAxisValue, int &yAxisValue);

// Setup potentiometer values:
const int masterPotResolution = 128;
const int masterPotMiddle = ceil (masterPotResolution * 0.5);
const int masterPotMinimum = 6;
const int masterPotMaximum = 122;

const byte potWriteAddress = 0x00;

// Slave Select Pins:
const int lsXOutPin = 21;
const int lsYOutPin = 20;
const int cXOutPin = 22;
const int cYOutPin = 23;

// Buttons:
const int lsLeft = 1;
const int lsRight = 5;
const int lsDown = 2;
const int lsUp = 3;
const int xMod1 = 4;
const int xMod2 = 6;
const int yMod1 = 7;
const int yMod2 = 8;
const int cLeft = 9;
const int cRight = 14;
const int cDown = 10;
const int cUp = 15;
const int tiltMod = 0;

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
double mod1Decimal = 0.3125;
double mod2Decimal = 0.4125;
double combinedMod1TiltDecimal = 0.6500;
double combinedMod2TiltDecimal = 0.8500;
double combinedModDecimal = 0.6500;
double tiltDecimal = 0.3500;

// C-Stick Y axis skew for angled smashes:
double cYAxisSkew = 0.6500;

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
    
    
    // Setup for Project M mode:
    bool xMod1IsPressed = !digitalRead (xMod1);
    bool yMod2IsPressed = !digitalRead (yMod2);
    
    if (xMod1IsPressed && yMod2IsPressed)
    {
        mod2Decimal = 0.5500;
        tiltDecimal = 0.4625;
    }
}

void loop()
{
    int lsXValue = getPotValue (lsLeft, lsRight, lsXOrder);
    int lsYValue = getPotValue (lsDown, lsUp, lsYOrder);
    int cXValue = getPotValue (cLeft, cRight, cXOrder);
    int cYValue = getPotValue (cDown, cUp, cYOrder);

    applyCStickModifiers (cXValue, cYValue, lsYValue);
    applyLeftStickModifiers (lsXValue, lsYValue);

    if (lsXValue != lsXLastValue)
    {
        writeToDigitalPot (lsXOutPin, lsXValue);
    }
    if (lsYValue != lsYLastValue)
    {
        writeToDigitalPot (lsYOutPin, lsYValue);
    }
    
    if (cXValue != cXLastValue)
    {
        writeToDigitalPot (cXOutPin, cXValue);
    }
    if (cYValue != cYLastValue)
    {
        writeToDigitalPot (cYOutPin, cYValue);
    }

    lsXLastValue = lsXValue;
    lsYLastValue = lsYValue;
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

void applyCStickModifiers (int cXAxisValue, int &cYAxisValue, int lsYAxisValue)
{
    bool tiltModIsPressed = !digitalRead (tiltMod);

    if (tiltModIsPressed
     && cXAxisValue != masterPotMiddle
     && lsYAxisValue != masterPotMiddle)
    {
        cYAxisValue = lsYAxisValue;
        applyModToPot (cYAxisValue, cYAxisSkew);
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

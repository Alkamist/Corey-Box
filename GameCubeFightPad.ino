#include <Bounce.h>
#include <SPI.h>

// Forward declarations:
void writeToDigitalPot (int writePin, int inputValue);
int getPotValue (int potLowPin, int potHighPin, int &potPressOrder);
void applyModToPot (int &inputPotValue, double inputModDecimal);
void applyCStickModifiers (int cXAxisValue, int &cYAxisValue, int lsYAxisValue);
void applyLeftStickModifiers (int &xAxisValue, int &yAxisValue);

// Setup potentiometer values:
const int masterPotResolution = 128;
const int masterPotMiddle = 64;
const int masterPotMinimum = 6;
const int masterPotMaximum = 122;

const byte potWriteAddress = 0x00;

// Slave Select Pins:
const int lsXOutPin = 21;
const int lsYOutPin = 20;
const int cXOutPin = 23;
const int cYOutPin = 22;

// Button pins:
const int lsLeft = 1;
const int lsRight = 4;
const int lsDown = 2;
const int lsUp = 3;

const int cLeft = 9;
const int cRight = 14;
const int cDown = 10;
const int cUp = 15;

const int xMod1 = 8;
const int xMod2 = 6;
const int yMod1 = 7;
const int yMod2 = 5;
const int tiltMod = 0;

// Extra buttons:
const int rButtonSignal = 16;
const int lButtonSignal = 17;
const int rButtonPin = 18;
const int lButtonPin = 19;

// Timers for extra buttons:
unsigned int extraButtonHoldTime = 108;
unsigned int lButtonDelayTime = 17;
unsigned int lButtonHoldTime = 17;
elapsedMillis rButtonHoldCounter;
elapsedMillis lButtonDelayCounter;

// Extra button setup:
Bounce rButton = Bounce (rButtonPin , 8);
Bounce lButton = Bounce (lButtonPin , 8);

// Extra button bools:
bool pressRButton = false;
bool pressRButtonPrevious = false;
bool rButtonIsPressed = false;
bool pressLButton = false;
bool pressLButtonPrevious = false;
bool lButtonIsPressed = false;
bool lButtonWasPressed = false;

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
int previousUnmoddedLsXValue = masterPotMiddle;
int previousUnmoddedLsYValue = masterPotMiddle;

// Modifier values:
double maximumPotDecimal = 0.95;
double mod1Decimal = 0.3125;
double mod2Decimal = 0.4250;
double combinedModDecimal = 0.7000;

double tiltDecimal = 0.3875;
double mod1TiltDecimal = 0.1875;
double mod2TiltDecimal = 0.2125;
double combinedModTiltDecimal = 0.2875;

// C-Stick Y axis skew for angled smashes:
double cYAxisSkew = 0.6500;

// Timers for maximum speed of updating pots:
const unsigned int maxPotUpdateSpeedInms = 1;
elapsedMillis lsXTimeCounter;
elapsedMillis lsYTimeCounter;
elapsedMillis cXTimeCounter;
elapsedMillis cYTimeCounter;

// Tilt timer set up:
unsigned int tiltTimeInms = 104;
elapsedMillis tiltTimeCounter;

// This function runs one time when you plug in the controller:
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

    SPI.begin();

    // Initialize the value of each potentiometer to the center:
    writeToDigitalPot (lsXOutPin, masterPotMiddle);
    writeToDigitalPot (lsYOutPin, masterPotMiddle);
    writeToDigitalPot (cXOutPin, masterPotMiddle);
    writeToDigitalPot (cYOutPin, masterPotMiddle);
        
    // Setup for Project M mode:
    bool xMod1IsPressed = !digitalRead (xMod1);
    bool yMod2IsPressed = !digitalRead (yMod2);

    // This initiates Project M mode if xMod1 and yMod2
    // are held down when you plug in the controller:
    if (xMod1IsPressed && yMod2IsPressed)
    {
        mod1Decimal = 0.3125;
        mod2Decimal = 0.5500;
        combinedModDecimal = 0.6750;

        tiltDecimal = 0.4625;
        mod1TiltDecimal = 0.2125;
        mod2TiltDecimal = 0.2250;
        combinedModTiltDecimal = 0.2875;
    }
}

// This is the main loop that is running every clock cycle:
void loop()
{
    // ---------------- Extra Buttons ----------------  
    rButton.update();
    lButton.update();

    if (rButton.fallingEdge())
    {
        rButtonIsPressed = true;
    }

    if (rButton.risingEdge())
    {
        rButtonIsPressed = false;

        if (!digitalRead (tiltMod))
            rButtonHoldCounter = 0;
    }
        
    if (rButtonHoldCounter <= extraButtonHoldTime
     || rButtonIsPressed)
        pressRButton = true;
    else
        pressRButton = false;

    if (pressRButton != pressRButtonPrevious)
    {
        if (pressRButton)
            pinMode (rButtonSignal, OUTPUT);
        else
            pinMode (rButtonSignal, INPUT);
    }

    pressRButtonPrevious = pressRButton;


    if (lButton.fallingEdge())
    {
        lButtonIsPressed = true;
        lButtonWasPressed = true;

        lButtonDelayCounter = 0;
    }

    if (lButton.risingEdge())
        lButtonIsPressed = false;
        
    if (lButtonDelayCounter >= lButtonDelayTime
    && (lButtonIsPressed || lButtonWasPressed))
    {
        if (lButtonIsPressed)
            pressLButton = true;

        if (lButtonWasPressed
         && lButtonDelayCounter <= (lButtonDelayTime + lButtonHoldTime))
            pressLButton = true;

        if (lButtonWasPressed
         && lButtonDelayCounter >= (lButtonDelayTime + lButtonHoldTime))
            lButtonWasPressed = false;
    }
    else
        pressLButton = false;

    if (pressLButton != pressLButtonPrevious)
    {
        if (pressLButton)
            pinMode (lButtonSignal, OUTPUT);
        else
            pinMode (lButtonSignal, INPUT);
    }

    pressLButtonPrevious = pressLButton;

    // -----------------------------------------------
    
    // Determine the numerical value of each pot based on button presses:
    int lsXValue = getPotValue (lsLeft, lsRight, lsXOrder);
    int lsYValue = getPotValue (lsDown, lsUp, lsYOrder);
    int cXValue = getPotValue (cLeft, cRight, cXOrder);
    int cYValue = getPotValue (cDown, cUp, cYOrder);

    if (lsXValue != previousUnmoddedLsXValue
     || lsYValue != previousUnmoddedLsYValue)
    {
        tiltTimeCounter = 0;
    }

    previousUnmoddedLsXValue = lsXValue;
    previousUnmoddedLsYValue = lsYValue;

    // Apply the modifers to each stick:
    applyCStickModifiers (cXValue, cYValue, lsYValue);
    applyLeftStickModifiers (lsXValue, lsYValue);

    // Write the values to each pot and get rid of redundant
    // values. Also check each pot timer to ensure we aren't
    // writing to the pots faster than our maximum write speed:
    if (lsXValue != lsXLastValue
     && lsXTimeCounter >= maxPotUpdateSpeedInms)
    {
        writeToDigitalPot (lsXOutPin, lsXValue);
        lsXTimeCounter = 0;
        lsXLastValue = lsXValue;      
    }
    if (lsYValue != lsYLastValue
     && lsYTimeCounter >= maxPotUpdateSpeedInms)
    {
        writeToDigitalPot (lsYOutPin, lsYValue);
        lsYTimeCounter = 0;
        lsYLastValue = lsYValue;       
    }
    
    if (cXValue != cXLastValue
     && cXTimeCounter >= maxPotUpdateSpeedInms)
    {
        writeToDigitalPot (cXOutPin, cXValue);
        cXTimeCounter = 0;
        cXLastValue = cXValue;
    }
    if (cYValue != cYLastValue
     && cYTimeCounter >= maxPotUpdateSpeedInms)
    {
        writeToDigitalPot (cYOutPin, cYValue);
        cYTimeCounter = 0;
        cYLastValue = cYValue;
    }
}

// This function writes to the given potentiometer:
void writeToDigitalPot (int writePin, int inputValue)
{
    digitalWrite (writePin, LOW);

    SPI.transfer (potWriteAddress);
    SPI.transfer (inputValue);

    digitalWrite (writePin, HIGH);
}

// This function determines the value of a given potentiometer
// based on order of button presses:
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

// This function applies a given modifier value to a given
// pot value:
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

// This function applies the c-stick modifiers.
// The current purpose of this is to allow for tilted
// smashes based on input from the left stick and tilt
// button:
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

// This function applies the left stick modifiers.
// There is an if statement and corresponding action
// for every single possibility of button presses that 
// has to do with the left stick:
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
        applyModToPot (yAxisValue, maximumPotDecimal);
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, mod2Decimal);
        applyModToPot (yAxisValue, maximumPotDecimal);
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, combinedModDecimal);
        applyModToPot (yAxisValue, maximumPotDecimal);
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        applyModToPot (xAxisValue, maximumPotDecimal);
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
        applyModToPot (xAxisValue, maximumPotDecimal);
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
        applyModToPot (xAxisValue, maximumPotDecimal);
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
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, maximumPotDecimal);
            applyModToPot (yAxisValue, maximumPotDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, tiltDecimal);
            applyModToPot (yAxisValue, tiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod1Decimal);
            applyModToPot (yAxisValue, maximumPotDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod1TiltDecimal);
            applyModToPot (yAxisValue, tiltDecimal);
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod2Decimal);
            applyModToPot (yAxisValue, maximumPotDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod2TiltDecimal);
            applyModToPot (yAxisValue, tiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, combinedModDecimal);
            applyModToPot (yAxisValue, maximumPotDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, combinedModTiltDecimal);
            applyModToPot (yAxisValue, tiltDecimal);
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, maximumPotDecimal);
            applyModToPot (yAxisValue, mod1Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, tiltDecimal);
            applyModToPot (yAxisValue, mod1TiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod1Decimal);
            applyModToPot (yAxisValue, mod1Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod1TiltDecimal);
            applyModToPot (yAxisValue, mod1TiltDecimal);
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod2Decimal);
            applyModToPot (yAxisValue, mod1Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod2TiltDecimal);
            applyModToPot (yAxisValue, mod1TiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, combinedModDecimal);
            applyModToPot (yAxisValue, mod1Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, combinedModTiltDecimal);
            applyModToPot (yAxisValue, mod1TiltDecimal);
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, maximumPotDecimal);
            applyModToPot (yAxisValue, mod2Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, tiltDecimal);
            applyModToPot (yAxisValue, mod2TiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod1Decimal);
            applyModToPot (yAxisValue, mod2Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod1TiltDecimal);
            applyModToPot (yAxisValue, mod2TiltDecimal);
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod2Decimal);
            applyModToPot (yAxisValue, mod2Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod2TiltDecimal);
            applyModToPot (yAxisValue, mod2TiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, combinedModDecimal);
            applyModToPot (yAxisValue, mod2Decimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, combinedModTiltDecimal);
            applyModToPot (yAxisValue, mod2TiltDecimal);
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, maximumPotDecimal);
            applyModToPot (yAxisValue, combinedModDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, tiltDecimal);
            applyModToPot (yAxisValue, combinedModTiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod1Decimal);
            applyModToPot (yAxisValue, combinedModDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod1TiltDecimal);
            applyModToPot (yAxisValue, combinedModTiltDecimal);
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, mod2Decimal);
            applyModToPot (yAxisValue, combinedModDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, mod2TiltDecimal);
            applyModToPot (yAxisValue, combinedModTiltDecimal);
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (tiltTimeCounter >= tiltTimeInms)
        {
            applyModToPot (xAxisValue, combinedModDecimal);
            applyModToPot (yAxisValue, combinedModDecimal);
            return;
        }
        else
        {
            applyModToPot (xAxisValue, combinedModTiltDecimal);
            applyModToPot (yAxisValue, combinedModTiltDecimal);
            return;
        }
    }
}

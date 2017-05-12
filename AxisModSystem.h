#ifndef AXISMODSYSTEM_H
#define AXISMODSYSTEM_H

// This is a class that holds modifier values
class ModList
{
public:

    ModList();
    ModList (double maximumValue,
             double mod1,
             double mod2,
             double combinedMod,
             double tilt,
             double mod1Tilt,
             double mod2Tilt,
             double combinedModTilt,
             double cYAxisSkew,
             unsigned int tiltTime);

    inline double getMaxiumumValue()   { return mMaximumValue; }
    inline double getMod1()            { return mMod1; }
    inline double getMod2()            { return mMod2; }
    inline double getCombinedMod()     { return mCombinedMod; }
    inline double getTilt()            { return mTilt; }
    inline double getMod1Tilt()        { return mMod1Tilt; }
    inline double getMod2Tilt()        { return mMod2Tilt; }
    inline double getCombinedModTilt() { return mCombinedModTilt; }
    inline double getCYAxisSkew()      { return mCYAxisSkew; }
    inline unsigned int getTiltTime()  { return mTiltTime; }

private:

    double mMaximumValue;
    double mMod1;
    double mMod2;
    double mCombinedMod;
    double mTilt;
    double mMod1Tilt;
    double mMod2Tilt;
    double mCombinedModTilt;

    // C-Stick Y axis skew for angled smashes:
    double mCYAxisSkew;

    unsigned int mTiltTime;

};

// Default Melee mod values for safety
ModList::ModList()
:  mMaximumValue (0.9500),
   mMod1 (0.3125),
   mMod2 (0.4250),
   mCombinedMod (0.7000),
   mTilt (0.3875),
   mMod1Tilt (0.1875),
   mMod2Tilt (0.2125),
   mCombinedModTilt (0.2875),
   mCYAxisSkew (0.6500),
   mTiltTime (104)
{}

// Use this constructor
ModList::ModList (double maximumValue,
                  double mod1,
                  double mod2,
                  double combinedMod,
                  double tilt,
                  double mod1Tilt,
                  double mod2Tilt,
                  double combinedModTilt,
                  double cYAxisSkew,
                  unsigned int tiltTime)
 : mMaximumValue (maximumValue),
   mMod1 (mod1),
   mMod2 (mod2),
   mCombinedMod (combinedMod),
   mTilt (tilt),
   mMod1Tilt (mod1Tilt),
   mMod2Tilt (mod2Tilt),
   mCombinedModTilt (combinedModTilt),
   mCYAxisSkew (cYAxisSkew),
   mTiltTime (tiltTime)
{}



class AxisModSystem
{
public:

    AxisModSystem (unsigned int tiltPin,
                   unsigned int xMod1Pin,
                   unsigned int xMod2Pin,
                   unsigned int yMod1Pin,
                   unsigned int yMod2Pin);

    void processCurrentValues();

    inline void setModList (ModList inputModList) { mModList = inputModList; }
    inline void resetTiltTimer() { mTiltTimeCounter = 0; }

    inline void setTiltTempDisableTime (unsigned int inputTime) { mTiltTempDisableTime = inputTime; }
    inline void tempDisableTilt() 
    { 
        mTiltTempDisabled = true; 
        mTiltTempDisableCounter = 0;
    }

    inline void setCurrentLsXValue (double inputValue) { mCurrentLsXValue = inputValue; }
    inline void setCurrentLsYValue (double inputValue) { mCurrentLsYValue = inputValue; }
    inline void setCurrentCXValue (double inputValue) { mCurrentCXValue = inputValue; }
    inline void setCurrentCYValue (double inputValue) { mCurrentCYValue = inputValue; }

    inline double getCurrentLsXValue() { return mCurrentLsXValue; }
    inline double getCurrentLsYValue() { return mCurrentLsYValue; }
    inline double getCurrentCXValue() { return mCurrentCXValue; }
    inline double getCurrentCYValue() { return mCurrentCYValue; }

private:

    ModList mModList;

    double mCurrentLsXValue;
    double mCurrentLsYValue;
    double mCurrentCXValue;
    double mCurrentCYValue;

    unsigned int mTiltPin;
    unsigned int mXMod1Pin;
    unsigned int mXMod2Pin;
    unsigned int mYMod1Pin;
    unsigned int mYMod2Pin;

    bool mTiltTempDisabled;
    unsigned int mTiltTempDisableTime;
    elapsedMillis mTiltTempDisableCounter;
    elapsedMillis mTiltTimeCounter;

    void appyModValue (double &axisValue, double mod);
    void applyCStickMods();
    void applyLeftStickModifiers();

};



AxisModSystem::AxisModSystem (unsigned int tiltPin,
                              unsigned int xMod1Pin,
                              unsigned int xMod2Pin,
                              unsigned int yMod1Pin,
                              unsigned int yMod2Pin)
 : mTiltPin (tiltPin),
   mXMod1Pin (xMod1Pin),
   mXMod2Pin (xMod2Pin),
   mYMod1Pin (yMod1Pin),
   mYMod2Pin (yMod2Pin),
   mCurrentLsXValue (0.50),
   mCurrentLsYValue (0.50),
   mCurrentCXValue (0.50),
   mCurrentCYValue (0.50),
   mTiltTempDisableTime (50),
   mTiltTempDisabled (false)
{}

void AxisModSystem::processCurrentValues()
{
    if (mTiltTempDisableCounter >= mTiltTempDisableTime)
        mTiltTempDisabled = false;
        
    applyLeftStickModifiers();
    applyCStickMods();
}

void AxisModSystem::appyModValue (double &axisValue, double mod)
{
    if (axisValue < 0.50)
    {
        axisValue = 0.50 - 0.50 * mod;
        return;
    }
    if (axisValue > 0.50)
    {
        axisValue = 0.50 + 0.50 * mod;
        return;
    }
}

// This function applies the c-stick modifiers.
// The current purpose of this is to allow for tilted
// smashes based on input from the left stick and tilt
// button:
void AxisModSystem::applyCStickMods()
{
    bool tiltModIsPressed = !digitalRead (mTiltPin);

    if (tiltModIsPressed
     && mCurrentCXValue != 0.50
     && mCurrentLsYValue != 0.50)
    {
        mCurrentCYValue = mCurrentLsYValue;
        appyModValue (mCurrentCYValue, mModList.getCYAxisSkew());
        return;
    }
}

// This function applies the left stick modifiers.
// There is an if statement and corresponding action
// for every single possibility of button presses that
// has to do with the left stick:
void AxisModSystem::applyLeftStickModifiers()
{
    bool xMod1IsPressed = !digitalRead (mXMod1Pin);
    bool xMod2IsPressed = !digitalRead (mXMod2Pin);
    bool yMod1IsPressed = !digitalRead (mYMod1Pin);
    bool yMod2IsPressed = !digitalRead (mYMod2Pin);
    bool tiltModIsPressed = !digitalRead (mTiltPin) && !mTiltTempDisabled;

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
        appyModValue (mCurrentLsXValue, mModList.getMod1());
        appyModValue (mCurrentLsYValue, mModList.getMaxiumumValue());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMod2());
        appyModValue (mCurrentLsYValue, mModList.getMaxiumumValue());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
        appyModValue (mCurrentLsYValue, mModList.getMaxiumumValue());
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMaxiumumValue());
        appyModValue (mCurrentLsYValue, mModList.getMod1());
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMod1());
        appyModValue (mCurrentLsYValue, mModList.getMod1());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMod2());
        appyModValue (mCurrentLsYValue, mModList.getMod1());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
        appyModValue (mCurrentLsYValue, mModList.getMod1());
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMaxiumumValue());
        appyModValue (mCurrentLsYValue, mModList.getMod2());
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMod1());
        appyModValue (mCurrentLsYValue, mModList.getMod2());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMod2());
        appyModValue (mCurrentLsYValue, mModList.getMod2());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
        appyModValue (mCurrentLsYValue, mModList.getMod2());
        return;
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMaxiumumValue());
        appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
        return;
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMod1());
        appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
        return;
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getMod2());
        appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
        return;
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && !tiltModIsPressed)
    {
        appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
        appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
        return;
    }

    // Tilt mod pressed:

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMaxiumumValue());
            appyModValue (mCurrentLsYValue, mModList.getMaxiumumValue());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getTilt());
            appyModValue (mCurrentLsYValue, mModList.getTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1());
            appyModValue (mCurrentLsYValue, mModList.getMaxiumumValue());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1Tilt());
            appyModValue (mCurrentLsYValue, mModList.getTilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2());
            appyModValue (mCurrentLsYValue, mModList.getMaxiumumValue());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2Tilt());
            appyModValue (mCurrentLsYValue, mModList.getTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
            appyModValue (mCurrentLsYValue, mModList.getMaxiumumValue());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedModTilt());
            appyModValue (mCurrentLsYValue, mModList.getTilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMaxiumumValue());
            appyModValue (mCurrentLsYValue, mModList.getMod1());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getTilt());
            appyModValue (mCurrentLsYValue, mModList.getMod1Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1());
            appyModValue (mCurrentLsYValue, mModList.getMod1());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1Tilt());
            appyModValue (mCurrentLsYValue, mModList.getMod1Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2());
            appyModValue (mCurrentLsYValue, mModList.getMod1());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2Tilt());
            appyModValue (mCurrentLsYValue, mModList.getMod1Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && !yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
            appyModValue (mCurrentLsYValue, mModList.getMod1());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedModTilt());
            appyModValue (mCurrentLsYValue, mModList.getMod1Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMaxiumumValue());
            appyModValue (mCurrentLsYValue, mModList.getMod2());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getTilt());
            appyModValue (mCurrentLsYValue, mModList.getMod2Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1());
            appyModValue (mCurrentLsYValue, mModList.getMod2());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1Tilt());
            appyModValue (mCurrentLsYValue, mModList.getMod2Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2());
            appyModValue (mCurrentLsYValue, mModList.getMod2());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2Tilt());
            appyModValue (mCurrentLsYValue, mModList.getMod2Tilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && !yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
            appyModValue (mCurrentLsYValue, mModList.getMod2());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedModTilt());
            appyModValue (mCurrentLsYValue, mModList.getMod2Tilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMaxiumumValue());
            appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getTilt());
            appyModValue (mCurrentLsYValue, mModList.getCombinedModTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && !xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1());
            appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod1Tilt());
            appyModValue (mCurrentLsYValue, mModList.getCombinedModTilt());
            return;
        }
    }

    if (!xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2());
            appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getMod2Tilt());
            appyModValue (mCurrentLsYValue, mModList.getCombinedModTilt());
            return;
        }
    }

    if (xMod1IsPressed
     && xMod2IsPressed
     && yMod1IsPressed
     && yMod2IsPressed
     && tiltModIsPressed)
    {
        if (mTiltTimeCounter >= mModList.getTiltTime())
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedMod());
            appyModValue (mCurrentLsYValue, mModList.getCombinedMod());
            return;
        }
        else
        {
            appyModValue (mCurrentLsXValue, mModList.getCombinedModTilt());
            appyModValue (mCurrentLsYValue, mModList.getCombinedModTilt());
            return;
        }
    }
}

#endif // AXISMODSYSTEM_H

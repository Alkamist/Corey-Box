#ifndef DIGIPOT_H
#define DIGIPOT_H

class DigiPot
{
public:

    DigiPot (unsigned int inputResolution, unsigned int inputPin);

    void init();
    void endLoop();

    inline void setRestPosition (double inputDecimal) { mRestPosition = getValueByDecimal (inputDecimal); }
    inline void setMinimum (double inputDecimal) { mMinimum = getValueByDecimal (inputDecimal); }
    inline void setMaximum (double inputDecimal) { mMaximum = getValueByDecimal (inputDecimal); }
    void setRange (double inputDecimal);
    void setCurrentValue (double inputDecimal);

    inline void setMaximumUpdateSpeed (unsigned int inputUpdateSpeed) { mMaximumUpdateSpeed = inputUpdateSpeed; }

    inline unsigned int getResolution() { return mResolution; }
    inline unsigned int getRestPosition() { return mRestPosition; }
    inline unsigned int getMinimum() { return mMinimum; }
    inline unsigned int getMaximum() { return mMaximum; }
    inline double getCurrentValue() { return mCurrentValue; }

private:

    unsigned int mResolution;
    unsigned int mRestPosition;
    unsigned int mMinimum;
    unsigned int mMaximum;
    unsigned int mCurrentValue;
    unsigned int mPreviousValue;

    unsigned int mMaximumUpdateSpeed;
    elapsedMillis mTimeCounter;

    bool mHasChanged;

    unsigned int mSlaveSelectPin;
    const byte mWriteAddress = 0x00;

    void writeCurrentValue();
    inline void setPreviousValue (double inputDecimal) { mPreviousValue = getValueByDecimal (inputDecimal); }
    unsigned int getValueByDecimal (double inputDecimal);

};



DigiPot::DigiPot (unsigned int inputResolution, unsigned int inputPin)
 : mResolution (inputResolution),
   mSlaveSelectPin (inputPin),
   mHasChanged (false)
{
    setRange (1.00);
    setRestPosition (0.50);
    setCurrentValue (0.50);
    setPreviousValue (0.50);
    setMaximumUpdateSpeed (1);
}

void DigiPot::init()
{
    writeCurrentValue();
}

void DigiPot::endLoop()
{
    if (mCurrentValue != mPreviousValue)
        mHasChanged = true;

    if (mHasChanged
     && mTimeCounter >= mMaximumUpdateSpeed)
    {
        writeCurrentValue();
        mTimeCounter = 0;
        mHasChanged = false;
    }

    mPreviousValue = mCurrentValue;
}

void DigiPot::writeCurrentValue()
{
    digitalWrite (mSlaveSelectPin, LOW);

    SPI.transfer (mWriteAddress);
    SPI.transfer (mCurrentValue);

    digitalWrite (mSlaveSelectPin, HIGH);
}

void DigiPot::setRange (double inputDecimal)
{
    if (inputDecimal > 1.00)
        inputDecimal = 1.00;
    if (inputDecimal < 0.50)
        inputDecimal = 0.50;

    double minimumDecimal = 1.00 - inputDecimal;

    mMinimum = getValueByDecimal (minimumDecimal);
    mMaximum = getValueByDecimal (inputDecimal);
}

void DigiPot::setCurrentValue (double inputDecimal)
{
    unsigned int newCurrentValue = getValueByDecimal (inputDecimal);

    if (newCurrentValue > mMaximum)
        newCurrentValue = mMaximum;
    if (newCurrentValue < mMinimum)
        newCurrentValue = mMinimum;

    mCurrentValue = newCurrentValue;
}

unsigned int DigiPot::getValueByDecimal (double inputDecimal)
{
    // Calculate the output value and round to an int.
    // The + 0.5 at the end ensures proper rounding
    // of positive numbers. Since we are using
    // an unsigned int that's ok.
    return mResolution * inputDecimal - 1 + 0.5;
}

#endif // DIGIPOT_H

#ifndef TWOBUTTONCONTROLLER_H
#define TWOBUTTONCONTROLLER_H

class TwoButtonController
{
public:

    TwoButtonController (unsigned int lowPin, unsigned int highPin);

    void processCurrentValue();

    inline double getCurrentValue() { return mCurrentValue; }

    inline bool hasChanged() { return mHasChanged; }

private:

    double mCurrentValue;
    double mPreviousValue;
    bool mHasChanged;

    unsigned int mLowPin;
    unsigned int mHighPin;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int mPressOrder = -1;

};



TwoButtonController::TwoButtonController (unsigned int lowPin, unsigned int highPin)
 : mLowPin (lowPin),
   mHighPin (highPin),
   mHasChanged (false)
{}

void TwoButtonController::processCurrentValue()
{
    bool lowIsPressed = !digitalRead (mLowPin);
    bool highIsPressed = !digitalRead (mHighPin);
    bool lowWasPressedFirst = lowIsPressed && (mPressOrder == 0);
    bool highWasPressedFirst = highIsPressed && (mPressOrder == 1);

    // Low Cases
    if (lowIsPressed && highWasPressedFirst)
    {
        mPressOrder = 1;
        mCurrentValue = 1.00;
    }
    if (lowIsPressed && !highIsPressed)
    {
        mPressOrder = 0;
        mCurrentValue = 0.0;
    }

    // High Cases
    if (lowWasPressedFirst && highIsPressed)
    {
        mPressOrder = 0;
        mCurrentValue = 1.00;
    }
    if (!lowIsPressed && highIsPressed)
    {
        mPressOrder = 1;
        mCurrentValue = 1.00;
    }

    // Middle Cases
    if (!lowIsPressed && !highIsPressed)
    {
        mPressOrder = -1;
        mCurrentValue = 0.50;
    }
    if (lowIsPressed && highIsPressed && (mPressOrder == -1))
    {
        mPressOrder = -1;
        mCurrentValue = 0.50;
    }

    if (mCurrentValue != mPreviousValue)
        mHasChanged = true;
    else
        mHasChanged = false;

    mPreviousValue = mCurrentValue;
}

#endif // TWOBUTTONCONTROLLER_H
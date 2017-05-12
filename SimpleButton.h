#ifndef SIMPLEBUTTON_H
#define SIMPLEBUTTON_H

class SimpleButton
{
public:

    SimpleButton (unsigned int pin);

    void update();

    inline bool isHeldDown()      { return mIsHeldDown; }
    inline bool wasJustPressed()  { return mWasJustPressed; }
    inline bool wasJustReleased() { return mWasJustReleased; }

    inline bool isHeldDownExtra()      { return mIsHeldDown; }
    inline bool wasJustReleasedExtra() { return mWasJustReleasedExtra; }

    inline void setExtraHoldTime (unsigned int extraTime) { mExtraHoldTime = extraTime; }

    inline elapsedMillis getStateTimer() { return mStateTimer; }

private:

    unsigned int mPin;

    bool mIsHeldDown;
    bool mWasPreviouslyHeldDown;
    bool mWasJustPressed;
    bool mWasJustReleased;

    bool mIsHeldDownExtra;
    bool mWasPreviouslyHeldDownExtra;
    bool mWasJustReleasedExtra;

    elapsedMillis mStateTimer;
    elapsedMillis mExtraHoldTimeCounter;
    unsigned int mExtraHoldTime;

};



SimpleButton::SimpleButton (unsigned int pin)
 : mPin (pin),
   mWasJustPressed (false),
   mWasJustReleased (false),
   mIsHeldDown (false),
   mWasPreviouslyHeldDown (false),
   mExtraHoldTime (0)
{}

void SimpleButton::update()
{
    mIsHeldDown = !digitalRead (mPin);

    if (mIsHeldDown)
    {
        mWasJustReleased = false;

        if (mWasPreviouslyHeldDown)
            mWasJustPressed = false;
        else
            mWasJustPressed = true;
    }
    else
    {
        mWasJustPressed = false;

        if (mWasPreviouslyHeldDown)
            mWasJustReleased = true;
        else
            mWasJustReleased = false;
    }

    if (mWasJustPressed || mWasJustReleased)
        mStateTimer = 0;

    mWasPreviouslyHeldDown = mIsHeldDown;

    // =============== Extra Time ===============

    if (mWasJustReleased)
    {
        mIsHeldDownExtra = true;
        mExtraHoldTimeCounter = 0;
    }

    if (mExtraHoldTimeCounter > mExtraHoldTime)
        mIsHeldDownExtra = false;

    if ((mIsHeldDownExtra == false)
     && (mWasPreviouslyHeldDownExtra == true))
        mWasJustReleasedExtra = true;
    else
        mWasJustReleasedExtra = false;

    mWasPreviouslyHeldDownExtra = mIsHeldDownExtra;
}

#endif // SIMPLEBUTTON_H
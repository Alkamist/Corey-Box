#ifndef EXTRABUTTONSYSTEM_H
#define EXTRABUTTONSYSTEM_H

#include "SimpleButton.h"

class ExtraButtonSystem
{
public:

    ExtraButtonSystem (unsigned int lButtonPin,
                       unsigned int lButtonSignalPin,
                       unsigned int rButtonPin,
                       unsigned int rButtonSignalPin,
                       unsigned int tiltButtonPin);

    void update();

    inline bool tiltIsTempDisabled() { return mTempDisableTilt; }
    inline SimpleButton& getRButton() { return mRButton; }

private:

    SimpleButton mRButton;
    SimpleButton mLButton;
    SimpleButton mTiltButton;

    unsigned int mLButtonSignalPin;
    unsigned int mRButtonSignalPin;

    bool mTempDisableTilt;
    
    void processButtons();
    
};



ExtraButtonSystem::ExtraButtonSystem (unsigned int lButtonPin,
                                      unsigned int lButtonSignalPin,
                                      unsigned int rButtonPin,
                                      unsigned int rButtonSignalPin,
                                      unsigned int tiltButtonPin)
 : mLButton (lButtonPin),
   mLButtonSignalPin (lButtonSignalPin),
   mRButton (rButtonPin),
   mRButtonSignalPin (rButtonSignalPin),
   mTiltButton (tiltButtonPin),
   mTempDisableTilt (false)
{}

void ExtraButtonSystem::update()
{
    mLButton.update();
    mRButton.update();
    mTiltButton.update();
    
    processButtons();
}

void ExtraButtonSystem::processButtons()
{
    if (mRButton.wasJustPressed())
        pinMode (mRButtonSignalPin, OUTPUT);
        
    if (!mRButton.isHeldDown())
    {
        if (mRButton.wasJustReleased() && !mTiltButton.isHeldDown())
        {
            pinMode (mRButtonSignalPin, INPUT);
            return;
        }

        if (mRButton.wasJustReleasedExtra())
        {
            pinMode (mRButtonSignalPin, INPUT);
            return;
        }
    }

    if (mLButton.wasJustPressed())
    {
        mTempDisableTilt = true;
        pinMode (mLButtonSignalPin, OUTPUT);
    }
    else
        mTempDisableTilt = false;
        
    if (mLButton.wasJustReleased())
        pinMode (mLButtonSignalPin, INPUT);
}
    
#endif // EXTRABUTTONSYSTEM_H

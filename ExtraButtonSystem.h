#ifndef EXTRABUTTONSYSTEM_H
#define EXTRABUTTONSYSTEM_H

#include "SimpleButton.h"
#include "SpamButton.h"

class ExtraButtonSystem
{
public:
    ExtraButtonSystem (unsigned int lButtonPin,
                       unsigned int lButtonSignalPin,
                       unsigned int rButtonPin,
                       unsigned int rButtonSignalPin,
                       unsigned int tiltButtonPin);

    void update();

    inline bool tiltIsTempDisabled() { return _tempDisableTilt; }
    inline SimpleButton& getRButton() { return _rButton; }
private:
    SimpleButton _rButton;
    SpamButton _lButton;
    SimpleButton _tiltButton;

    unsigned int _lButtonSignalPin;
    unsigned int _rButtonSignalPin;

    bool _tempDisableTilt;
    bool _rIsLetGo;

    void processButtons();
};



ExtraButtonSystem::ExtraButtonSystem (unsigned int lButtonPin,
                                      unsigned int lButtonSignalPin,
                                      unsigned int rButtonPin,
                                      unsigned int rButtonSignalPin,
                                      unsigned int tiltButtonPin)
 : _lButton(lButtonPin),
   _lButtonSignalPin(lButtonSignalPin),
   _rButton(rButtonPin),
   _rButtonSignalPin(rButtonSignalPin),
   _tiltButton(tiltButtonPin),
   _tempDisableTilt(false),
   _rIsLetGo(false)
{
    _lButton.setSpamSpeedHz(30);
    _lButton.setSpamCount(2);
}

void ExtraButtonSystem::update()
{
    _lButton.update();
    _rButton.update();
    _tiltButton.update();

    processButtons();
}

void ExtraButtonSystem::processButtons()
{
    if (_rButton.wasJustPressed())
        pinMode (_rButtonSignalPin, OUTPUT);

    if (!_rButton.isHeldDown())
    {
        if (_rButton.wasJustReleased() && !_tiltButton.isHeldDown())
        {
            pinMode (_rButtonSignalPin, INPUT);
            return;
        }

        if (_rButton.wasJustReleasedExtra())
        {
            pinMode (_rButtonSignalPin, INPUT);
            return;
        }
    }

    // Let go of R after spamming if it is not held down
    if (!_lButton.isSpamming()
     && !_rButton.isHeldDown()
      && _rIsLetGo == false)
    {
        pinMode (_rButtonSignalPin, INPUT);
        _rIsLetGo = true;
    }

    // Temporarily disable tilt if L is pressed
    if (_lButton.wasJustPressed())
        _tempDisableTilt = true;
    else
        _tempDisableTilt = false;

    // Alternate between spamming L and R
    if (_lButton.spam_wasJustPressed())
    {
        pinMode (_lButtonSignalPin, OUTPUT);
        pinMode (_rButtonSignalPin, INPUT);
    }

    else if (_lButton.spam_wasJustReleased())
    {
        pinMode (_lButtonSignalPin, INPUT);
        pinMode (_rButtonSignalPin, OUTPUT);
        _rIsLetGo = false;
    }
}

#endif // EXTRABUTTONSYSTEM_H

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

    inline bool tiltIsTempDisabled() { return _tempDisableTilt; }
    inline SimpleButton& getRButton() { return _rButton; }
private:
    SimpleButton _rButton;
    SimpleButton _lButton;
    SimpleButton _tiltButton;

    unsigned int _lButtonSignalPin;
    unsigned int _rButtonSignalPin;

    bool _tempDisableTilt;

    void processButtons();
};



ExtraButtonSystem::ExtraButtonSystem (unsigned int lButtonPin,
                                      unsigned int lButtonSignalPin,
                                      unsigned int rButtonPin,
                                      unsigned int rButtonSignalPin,
                                      unsigned int tiltButtonPin)
 : _lButton (lButtonPin),
   _lButtonSignalPin (lButtonSignalPin),
   _rButton (rButtonPin),
   _rButtonSignalPin (rButtonSignalPin),
   _tiltButton (tiltButtonPin),
   _tempDisableTilt (false)
{}

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

    if (_lButton.wasJustPressed())
    {
        _tempDisableTilt = true;
        pinMode (_lButtonSignalPin, OUTPUT);
    }
    else
        _tempDisableTilt = false;

    if (_lButton.wasJustReleased())
        pinMode (_lButtonSignalPin, INPUT);
}

#endif // EXTRABUTTONSYSTEM_H
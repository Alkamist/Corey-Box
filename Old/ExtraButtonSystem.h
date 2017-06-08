#ifndef EXTRABUTTONSYSTEM_H
#define EXTRABUTTONSYSTEM_H

#include "Globals.h"
#include "MasterButton.h"
#include "UpdatedBool.h"

// This class handles the interactions between L, R, and Tilt
class ExtraButtonSystem
{
public:
    ExtraButtonSystem();

    void update();

    inline bool tiltIsTempDisabled() { return _tempDisableTilt; }
private:
    MasterButton _rButton;
    MasterButton _lButton;
    MasterButton _tiltButton;

    UpdatedBool _lPressed;
    UpdatedBool _rPressed;

    bool _tempDisableTilt;
    bool _lSpamStarted;

    void processButtons();
    void handleButtonOutput();
};



ExtraButtonSystem::ExtraButtonSystem()
 : _tempDisableTilt(false),
   _lSpamStarted(false)
{}

void ExtraButtonSystem::update()
{
    _tiltButton.simpleButton().setValue(!TILT_BOUNCE.read());
    _lButton.simpleButton().setValue(!L_BUTTON_BOUNCE.read());
    _rButton.simpleButton().setValue(!R_BUTTON_BOUNCE.read());

    _lButton.update();
    _rButton.update();
    _tiltButton.update();

    _lPressed.update();
    _rPressed.update();

    processButtons();
}

// This function handles the annoying logic for the L, R,
// and Tilt buttons. R acts normally, but if tilt is held
// down while R is released, R will hold down for a little
// bit longer. L is complicated. If you push down L, it
// alternate spamming L and R every frame for the amount
// of pressed specified.
void ExtraButtonSystem::processButtons()
{
    int speedInHz = 30;
    int lSpamCount = 1;
    int rSpamCount = 1;

    if (_lButton.simpleButton().justPressed())
    {
        _lButton.spamButton().spam(lSpamCount, speedInHz);

        _lSpamStarted = true;

        _tempDisableTilt = true;
    }
    else
        _tempDisableTilt = false;

    if (_lButton.spamButton().firstPressJustEnded())
    {
        _lSpamStarted = false;
        _rButton.spamButton().spam(rSpamCount, speedInHz);
    }

    if (_rButton.simpleButton().justReleased()
     && _tiltButton.simpleButton().pressed())
        _rButton.holdButton().hold(R_EXTRA_HOLD_TIME);

    //===================================================

    if (_lButton.spamButton().pressed())
        _lPressed.set(true);
    else
        _lPressed.set(false);

    if ((!_rButton.spamButton().lastPressEnded()
       || _lSpamStarted)
       && rSpamCount > 0)
    {
        if (_rButton.spamButton().pressed())
            _rPressed.set(true);
        else
            _rPressed.set(false);
    }
    else
    {
        if (_rButton.simpleButton().pressed()
         || _rButton.holdButton().pressed())
            _rPressed.set(true);
        else
            _rPressed.set(false);
    }

    handleButtonOutput();
}

// This function simply handles the output of the result
// of the processButtons() function.
void ExtraButtonSystem::handleButtonOutput()
{
    if (_lPressed.hasChanged())
    {
        if (_lPressed.isTrue())
            pinMode(L_BUTTON_SIGNAL_PIN, OUTPUT);
        else
            pinMode(L_BUTTON_SIGNAL_PIN, INPUT);
    }

    if (_rPressed.hasChanged())
    {
        if (_rPressed.isTrue())
            pinMode(R_BUTTON_SIGNAL_PIN, OUTPUT);
        else
            pinMode(R_BUTTON_SIGNAL_PIN, INPUT);
    }
}

#endif // EXTRABUTTONSYSTEM_H
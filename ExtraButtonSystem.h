#ifndef EXTRABUTTONSYSTEM_H
#define EXTRABUTTONSYSTEM_H

#include "Globals.h"
#include "SimpleButton.h"
#include "SpamButton.h"

class ExtraButtonSystem
{
public:
    ExtraButtonSystem ();

    void update();

    inline bool tiltIsTempDisabled() { return _tempDisableTilt; }
    inline SimpleButton& getRButton() { return _rButton; }
private:
    SimpleButton _rButton;
    SpamButton _lButton;
    SimpleButton _tiltButton;

    bool _tempDisableTilt;
    bool _rIsLetGo;
    bool _lIsLetGo;

    void processButtons();
};



ExtraButtonSystem::ExtraButtonSystem ()
 : _lButton(L_BUTTON_PIN),
   _rButton(R_BUTTON_PIN),
   _tiltButton(TILT_MOD_PIN),
   _tempDisableTilt(false),
   _rIsLetGo(false),
   _lIsLetGo(false)
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
        pinMode (R_BUTTON_SIGNAL_PIN, OUTPUT);

    if (!_rButton.isHeldDown())
    {
        if (_rButton.wasJustReleased() && !_tiltButton.isHeldDown())
        {
            pinMode (R_BUTTON_SIGNAL_PIN, INPUT);
            return;
        }

        if (_rButton.wasJustReleasedExtra())
        {
            pinMode (R_BUTTON_SIGNAL_PIN, INPUT);
            return;
        }
    }

    // Let go of R after spamming if it is not held down
    if (!_lButton.isSpamming()
     && !_rButton.isHeldDown()
     && _rIsLetGo == false)
    {
        pinMode (R_BUTTON_SIGNAL_PIN, INPUT);
        _rIsLetGo = true;
    }

    // Let go of L after spamming if it is not held down
    if (!_lButton.isSpamming()
     && !_lButton.isHeldDown()
     && _lIsLetGo == false)
    {
        pinMode (L_BUTTON_SIGNAL_PIN, INPUT);
        _lIsLetGo = true;
    }

    // Temporarily disable tilt if L is pressed
    if (_lButton.wasJustPressed())
    {
        _tempDisableTilt = true;
        _lIsLetGo = false;
    }
    else
        _tempDisableTilt = false;

    bool spamOverrideCombo = !digitalRead (X_MOD1_PIN)
                          && !digitalRead (Y_MOD2_PIN);

    if (spamOverrideCombo)
    {
        if (_lButton.wasJustPressed())
            pinMode (L_BUTTON_SIGNAL_PIN, OUTPUT);
    }
    else
    {
        // Alternate between spamming L and R
        if (_lButton.spam_wasJustPressed())
        {
            pinMode (L_BUTTON_SIGNAL_PIN, OUTPUT);
            pinMode (R_BUTTON_SIGNAL_PIN, INPUT);
        }
        else if (_lButton.spam_wasJustReleased())
        {
            pinMode (L_BUTTON_SIGNAL_PIN, INPUT);
            pinMode (R_BUTTON_SIGNAL_PIN, OUTPUT);
            _rIsLetGo = false;
        }
    }
}

#endif // EXTRABUTTONSYSTEM_H

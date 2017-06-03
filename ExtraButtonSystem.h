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
    inline MasterButton& getRButton() { return _rButton; }
private:
    MasterButton _rButton;
    MasterButton _lButton;
    MasterButton _tiltButton;

    UpdatedBool _lPressed;
    UpdatedBool _rPressed;

    bool _tempDisableTilt;

    void processButtons();
    void handleButtonOutput();
};



ExtraButtonSystem::ExtraButtonSystem()
 : _tempDisableTilt(false)
{}

void ExtraButtonSystem::update()
{
    processButtons();

    _lButton.simpleButton().setValue(!digitalRead(L_BUTTON_PIN));
    _rButton.simpleButton().setValue(!digitalRead(R_BUTTON_PIN));

    _lButton.update();
    _rButton.update();
    _tiltButton.update();

    _lPressed.update();
    _rPressed.update();
}

void ExtraButtonSystem::processButtons()
{
    // R

    if (_rButton.simpleButton().pressed())
        _rPressed.set(true);
    else
        _rPressed.set(false);

    // L

    if (_lButton.simpleButton().pressed())
        _lPressed.set(true);
    else
        _lPressed.set(false);

    // Temporarily disable tilt if L is pressed
    if (_lButton.simpleButton().justPressed())
        _tempDisableTilt = true;
    else
        _tempDisableTilt = false;

    handleButtonOutput();
}

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

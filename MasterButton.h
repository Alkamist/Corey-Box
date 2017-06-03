#ifndef MASTERBUTTON_H
#define MASTERBUTTON_H

#include "SimpleButton.h"
#include "HoldButton.h"
#include "SpamButton.h"

// This class collects the functionality of all the types of
// buttons into a single class. Use this for all of the main
// buttons and access the components as you need them.
class MasterButton
{
public:
    MasterButton();

    void update();

    inline SimpleButton& simpleButton() { return _simpleButton; }
    inline HoldButton&   holdButton()   { return _holdButton; }
    inline SpamButton&   spamButton()   { return _spamButton; }

private:
    SimpleButton _simpleButton;
    HoldButton   _holdButton;
    SpamButton   _spamButton;
};



MasterButton::MasterButton() {}

void MasterButton::update()
{
    _simpleButton.update();
    _holdButton.update();
    _spamButton.update();
}

#endif // MASTERBUTTON_H
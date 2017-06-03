#ifndef HOLDBUTTON_H
#define HOLDBUTTON_H

#include "SimpleButton.h"

// This class represents a button that will hold for a
// set period of time on command.
class HoldButton : public SimpleButton
{
public:
    HoldButton();

    void update();

    void hold(unsigned int millisToHold);

private:
    unsigned int _millisToHold;
};



HoldButton::HoldButton()
 : _millisToHold(0)
{}

void HoldButton::hold(unsigned int millisToHold)
{
    _millisToHold = millisToHold;
    resetTimer();
}

void HoldButton::update()
{
    if (getTimer() < _millisToHold)
        setValue(true);
    else
        setValue(false);

    SimpleButton::update();
}

#endif // HOLDBUTTON_H
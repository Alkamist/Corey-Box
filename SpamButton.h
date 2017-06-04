#ifndef SPAMBUTTON_H
#define SPAMBUTTON_H

#include "SimpleButton.h"

// This class represents a button that will spam itself
// a given number of times at a given speed.
class SpamButton : public SimpleButton
{
public:
    SpamButton();

    void update();

    void spam(unsigned int numPresses, unsigned int speedInHz);

    inline bool isSpamming() { return _isSpamming; }

    inline unsigned int getSpamInterval() { return _spamInterval; }

private:
    unsigned int _spamInterval;
    unsigned int _spamCount;
    unsigned int _spamCounter;

    bool _isSpamming;
};



SpamButton::SpamButton()
 : _spamInterval(16),
   _spamCount(0),
   _spamCounter(0),
   _isSpamming(false)
 {}

void SpamButton::spam(unsigned int numPresses, unsigned int speedInHz)
{
    _spamCounter = 1;
    _spamCount = numPresses;
    _spamInterval = 1000 / (speedInHz * 2);

    setValue(true);
    _isSpamming = true;
    resetTimer();
}

void SpamButton::update()
{
    if (_spamCounter > _spamCount)
    {
        _isSpamming = false;
        setValue(false);
    }

    if ((getTimer() >= _spamInterval) && _isSpamming)
    {
        if (pressed())
        {
            setValue(false);
            resetTimer();
        }
        else
        {
            ++_spamCounter;
            setValue(true);
            resetTimer();
        }
    }

    SimpleButton::update();
}

#endif // SPAMBUTTON_H
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

private:
    unsigned int _spamInterval;
    unsigned int _spamCount;
    unsigned int _spamCounter;

    bool _startSpam;
};



SpamButton::SpamButton()
 : _spamInterval(16),
   _spamCount(0),
   _spamCounter(0),
   _startSpam(false)
 {}

void SpamButton::spam(unsigned int numPresses, unsigned int speedInHz)
{
    _spamCounter = 0;
    _spamCount = numPresses;
    _spamInterval = 1000 / (speedInHz * 2.0);

    _startSpam = true;
    resetTimer();
}

void SpamButton::update()
{
    if ((getTimer() >= _spamInterval && _spamCounter < _spamCount)
      || _startSpam)
    {
        ++_spamCounter;
        _startSpam = false;

        if (pressed())
            setValue(true);
        else
            setValue(false);
    }

    SimpleButton::update();
}

#endif // SPAMBUTTON_H
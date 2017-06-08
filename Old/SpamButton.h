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
    inline bool firstPressJustEnded() { return _firstPressJustEnded; }
    inline bool lastPressEnded() { return _lastPressEnded; }

private:
    unsigned int _spamInterval;
    unsigned int _spamCount;
    unsigned int _spamCounter;

    bool _firstPressJustEnded;
    bool _lastPressEnded;

    bool _isSpamming;
};



SpamButton::SpamButton()
 : _spamInterval(16),
   _spamCount(0),
   _spamCounter(0),
   _isSpamming(false),
   _firstPressJustEnded(false),
   _lastPressEnded(true)
 {}

void SpamButton::spam(unsigned int numPresses, unsigned int speedInHz)
{
    _spamCounter = 1;
    _spamCount = numPresses;
    _spamInterval = 1000 / (speedInHz * 2);

    setValue(true);
    _isSpamming = true;
    _lastPressEnded = false;
    resetTimer();
}

void SpamButton::update()
{
    _firstPressJustEnded = false;

    if (_spamCounter > _spamCount)
    {
        _isSpamming = false;
        _lastPressEnded = true;
        setValue(false);
    }

    if ((getTimer() >= _spamInterval) && _isSpamming)
    {
        if (pressed())
        {
            if (_spamCounter == 1)
                _firstPressJustEnded = true;

            if (_spamCounter == _spamCount)
                _lastPressEnded = true;

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
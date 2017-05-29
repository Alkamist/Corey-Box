#ifndef SPAMBUTTON_H
#define SPAMBUTTON_H

#include "SimpleButton.h"

class SpamButton : public SimpleButton
{
public:
    SpamButton (unsigned int pin);

    void update();

    inline void setSpamSpeedHz(unsigned int spamSpeed) { _spamInterval = 1000 / (spamSpeed * 2.0); };
    inline void setSpamCount(unsigned int spamCount) { _spamCount = spamCount; };

    inline bool isSpamming() { return _spam; };
    inline bool spam_isHeld() { return _spamHeld; };
    inline bool spam_wasJustPressed() { return _spamJustPressed; };
    inline bool spam_wasJustReleased() { return _spamJustReleased; };
private:
    bool _spamHeld;

    bool _spam;
    bool _spamJustPressed;
    bool _spamJustReleased;

    unsigned int _spamCount;
    unsigned int _spamCounter;

    unsigned int _spamInterval;
    unsigned int _spamReleaseTime;
    elapsedMillis _spamTimer;
    elapsedMillis _spamReleaseTimer;
};



SpamButton::SpamButton (unsigned int pin)
 : SimpleButton(pin),
   _spamHeld(false),
   _spamJustPressed(false),
   _spamJustReleased(false),
   _spam(false),
   _spamInterval(33),
   _spamCount(0),
   _spamReleaseTime(16)
{}

void SpamButton::update()
{
    SimpleButton::update();

    _spamJustPressed = false;
    _spamJustReleased = false;

    if (wasJustReleased())
        _spamReleaseTimer = 0;

    if (wasJustPressed()
     && _spamReleaseTimer > _spamReleaseTime)
    {
        _spam = true;
        _spamCounter = 1;
        _spamTimer = 0;
        _spamHeld = true;
        _spamJustPressed = true;
    }

    if (_spam
     && _spamTimer > _spamInterval)
    {
        // Check if we have exceeded spam presses
        if (_spamCounter >= _spamCount)
        {
            _spamHeld = false;
            _spamJustReleased = true;
            _spam = false;
            _spamCounter = 0;
        }
        // Check if we need to push down
        else if (_spamHeld == false)
        {
            _spamHeld = true;
            _spamJustPressed = true;
            _spamTimer = 0;
            ++_spamCounter;
        }
        // Check if we need to release
        else if (_spamHeld == true)
        {
            _spamHeld = false;
            _spamJustReleased = true;
            _spamTimer = 0;
        }
    }
}

#endif // SPAMBUTTON_H
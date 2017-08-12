#ifndef BOUNCEBUTTON_H
#define BOUNCEBUTTON_H

#include <Bounce2.h>
#include "Activator.h"

// This is the basic class to read input from a button. It uses
// the Bounce2 library to reduce button chatter. This prevents
// things like the wavedash macro from triggering accidentally
// when you release the button.
class BounceButton : public Activator
{
public:
    explicit BounceButton(const uint8_t pin)
    : Activator(),
      _pin(pin)
    {
        init();
    }

    void endCycle()
    {
        Activator::endCycle();
        _bounce.update();

        *this = !_bounce.read();
    }

private:
    uint8_t _pin;

    Bounce _bounce;

    void init()
    {
        pinMode(_pin, INPUT_PULLUP);

        _bounce.attach(_pin);
        _bounce.interval(1);
    }

    Activator& operator =(const bool value) { Activator::operator=(value); return *this; }
};

#endif // BOUNCEBUTTON_H
#ifndef BUTTONREADER_H
#define BUTTONREADER_H

#include <Bounce2.h>
#include "Activator.h"

// This is the basic class to read input from a button.
class ButtonReader : public Activator
{
public:
    explicit ButtonReader(const uint8_t pin)
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
        _bounce.interval(5);
    }

    Activator& operator =(const bool value) { Activator::operator=(value); return *this; }
};

#endif // BUTTONREADER_H
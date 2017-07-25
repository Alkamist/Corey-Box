#ifndef BUTTONREADER_H
#define BUTTONREADER_H

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

        *this = !digitalRead(_pin);
    }

private:
    uint8_t _pin;

    void init()
    {
        pinMode(_pin, INPUT_PULLUP);
    }

    Activator& operator =(const bool value) { Activator::operator=(value); return *this; }
};

#endif // BUTTONREADER_H
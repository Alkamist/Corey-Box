#ifndef BUTTONREADER_H
#define BUTTONREADER_H

#include <Bounce2.h>
#include "Activator.h"

// This is the basic class to read input from a button. You can
// set it to use the bounce library to reduce button chatter.
class ButtonReader : public Activator
{
public:
    explicit ButtonReader(const uint8_t pin);

    static void setUseBounce(const bool state) { _useBounce = state; }

    void endCycle();

private:
    uint8_t _pin;

    static bool _useBounce;

    Bounce _bounce;

    Activator& operator =(const bool value) { Activator::operator=(value); return *this; }
};

#endif // BUTTONREADER_H
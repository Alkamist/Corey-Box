#ifndef BUTTONREADER_H
#define BUTTONREADER_H

#include <Bounce2.h>
#include "ControlValue.h"

// This class is a ControlValue that is tied to the
// Bounce library. It is useful to accurately read
// the value of a pin that is connected to a physical
// button.
class ButtonReader : public ControlValue
{
public:
    explicit ButtonReader(const unsigned int pin);

    void init();
    void update();

private:
    unsigned int _pin;

    Bounce _bounce;
};



void ButtonReader::init()
{
    _bounce.attach(_pin);
    _bounce.interval(5);
}

void ButtonReader::update()
{
    _bounce.update();
    ControlValue::update();

    setValue(!_bounce.read());
}

ButtonReader::ButtonReader(const unsigned int pin)
: ControlValue(),
  _pin(pin)
{
    pinMode(_pin, INPUT_PULLUP);

    init();
}

#endif // BUTTONREADER_H
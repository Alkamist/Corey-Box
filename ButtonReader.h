#ifndef BUTTONREADER_H
#define BUTTONREADER_H

#include <Bounce2.h>
#include "Control.h"

class ButtonReader : public Control
{
public:
    explicit ButtonReader(const unsigned int pin);

    virtual void update();

    void init();

private:
    unsigned int _pin;

    Bounce _bounce;
};



void ButtonReader::update()
{
    _bounce.update();
    Control::update();

    setValue(!_bounce.read());
}

void ButtonReader::init()
{
    pinMode(_pin, INPUT_PULLUP);

    _bounce.attach(_pin);
    _bounce.interval(5);
}

ButtonReader::ButtonReader(const unsigned int pin)
: Control(),
  _pin(pin)
{
    init();
}

#endif // BUTTONREADER_H
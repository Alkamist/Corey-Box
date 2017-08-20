#include "Arduino.h"
#include "ButtonReader.h"

bool ButtonReader::_useBounce = false;

ButtonReader::ButtonReader(const uint8_t pin)
: Activator(),
  _pin(pin)
{
    pinMode(_pin, INPUT_PULLUP);

    _bounce.attach(_pin);
    _bounce.interval(1);
}

void ButtonReader::endCycle()
{
    Activator::endCycle();

    if (_useBounce)
    {
        _bounce.update();

        *this = !_bounce.read();
    }
    else
        *this = !digitalRead(_pin);
}
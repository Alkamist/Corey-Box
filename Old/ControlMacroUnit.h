#ifndef CONTROLMACROUNIT_H
#define CONTROLMACROUNIT_H

#include "ControlValue.h"

struct ControlMacroUnit : public ControlValue
{
public:
    explicit ControlMacroUnit(const ControlValue value,
                              const unsigned int delay);

    void setDelay(const unsigned int delay) { _delay = delay; }
    const unsigned int getDelay() const     { return _delay; }

private:
    unsigned int _delay;
};



ControlMacroUnit::ControlMacroUnit(const ControlValue value,
                                   const unsigned int delay)
: ControlValue(value),
  _delay(delay)
{}

#endif // CONTROLMACROUNIT_H
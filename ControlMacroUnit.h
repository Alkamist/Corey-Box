#ifndef CONTROLMACROUNIT_H
#define CONTROLMACROUNIT_H

#include "ControlValue.h"

struct ControlMacroUnit : public ControlValue
{
public:
    ControlMacroUnit();
    explicit ControlMacroUnit(const double value,
                              const unsigned int duration);

    void setDuration(const unsigned int duration) { _duration = duration; }
    const unsigned int getDuration() const        { return _duration; }

private:
    unsigned int _duration;
};



ControlMacroUnit::ControlMacroUnit()
: ControlValue(),
  _duration(0)
{}

ControlMacroUnit::ControlMacroUnit(const double value,
                                   const unsigned int duration)
: ControlValue(value),
  _duration(duration)
{}

#endif // CONTROLMACROUNIT_H
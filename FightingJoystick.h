#ifndef FIGHTINGJOYSTICK_H
#define FIGHTINGJOYSTICK_H

#include "Joystick.h"
#include "Range.h"

// This class is just a joystick that has some defined zones that relate
// to fighting games, particularly Melee.
class FightingJoystick : public Joystick
{
public:
    FightingJoystick()
    {
        _deadZone.setLowerBound(0);
        _deadZone.setUpperBound(22);
    }

    void setDeadZoneUpperBound(const uint8_t value) { _deadZone.setUpperBound(value); }

    const bool xIsInDeadZone() const                { return _deadZone.checkIfInRange(xValue.getBipolarMagnitude()); }
    const bool yIsInDeadZone() const                { return _deadZone.checkIfInRange(yValue.getBipolarMagnitude()); }

    const uint8_t getDeadZoneUpperBound() const     { return _deadZone.getUpperBound(); }

private:
    Range<uint8_t> _deadZone;
};

#endif // FIGHTINGJOYSTICK_H

#ifndef FIGHTINGJOYSTICK_H
#define FIGHTINGJOYSTICK_H

#include "Joystick.h"
#include "Range.h"

class FightingJoystick : public Joystick
{
public:
    FightingJoystick()
    {
        _deadZone.setLowerBound(0);
        _deadZone.setUpperBound(36);

        _tiltZone.setLowerBound(37);
        _tiltZone.setUpperBound(100);

        _smashZone.setLowerBound(101);
        _smashZone.setUpperBound(128);

        _crouchZone.setLowerBound(0);
        _crouchZone.setUpperBound(50);
    }

    const bool xIsInDeadZone() const   { return _deadZone.checkIfInRange(xValue.getBipolarMagnitude()); }
    const bool yIsInDeadZone() const   { return _deadZone.checkIfInRange(yValue.getBipolarMagnitude()); }

    const bool xIsInTiltZone() const   { return _tiltZone.checkIfInRange(xValue.getBipolarMagnitude()); }

    const bool yIsInCrouchZone() const { return _crouchZone.checkIfInRange(yValue); }

    const uint8_t getDeadZoneUpperBound() const { return _deadZone.getUpperBound(); }

private:
    Range<uint8_t> _deadZone;
    Range<uint8_t> _tiltZone;
    Range<uint8_t> _smashZone;
    Range<uint8_t> _crouchZone;
};

#endif // FIGHTINGJOYSTICK_H

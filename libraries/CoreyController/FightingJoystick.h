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

        _tiltZone.setLowerBound(23);
        _tiltZone.setUpperBound(63);

        _smashZone.setLowerBound(64);
        _smashZone.setUpperBound(128);

        _crouchZone.setLowerBound(0);
        _crouchZone.setUpperBound(77);
    }

    void setDeadZoneUpperBound(const uint8_t value) { _deadZone.setUpperBound(value); _tiltZone.setLowerBound(value + 1); }

    const bool xIsInDeadZone() const                { return _deadZone.checkIfInRange(xValue.getBipolarMagnitude()); }
    const bool yIsInDeadZone() const                { return _deadZone.checkIfInRange(yValue.getBipolarMagnitude()); }

    const bool xJustLeftDeadZone() const
    {
        return !_deadZone.checkIfInRange(xValue.getBipolarMagnitude()) && _deadZone.checkIfInRange(xValue.getPreviousBipolarMagnitude());
    }

    const bool yJustLeftDeadZone() const
    {
        return !_deadZone.checkIfInRange(yValue.getBipolarMagnitude()) && _deadZone.checkIfInRange(yValue.getPreviousBipolarMagnitude());
    }

    const bool xIsInTiltZone() const                { return _tiltZone.checkIfInRange(xValue.getBipolarMagnitude()); }

    const bool yIsInCrouchZone() const              { return _crouchZone.checkIfInRange(yValue); }

    const uint8_t getDeadZoneUpperBound() const     { return _deadZone.getUpperBound(); }
    const uint8_t getTiltLowerBound() const         { return _tiltZone.getLowerBound(); }
    const uint8_t getCrouchZoneUpperBound() const   { return _crouchZone.getUpperBound(); }

private:
    Range<uint8_t> _deadZone;
    Range<uint8_t> _tiltZone;
    Range<uint8_t> _smashZone;
    Range<uint8_t> _crouchZone;
};

#endif // FIGHTINGJOYSTICK_H

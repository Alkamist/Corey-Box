#ifndef DEADZONE_H
#define DEADZONE_H

class DeadZone
{
public:
    DeadZone();
    DeadZone(const double center,
             const double range);

    const bool check(const double value) const;

    const double getCenter() const     { return _center; }
    const double getLowerBound() const { return _center - _range / 2.0; }
    const double getUpperBound() const { return _center + _range / 2.0; }
    const double getRange() const      { return _range; }

    void setCenter(const double value) { _center = value; }
    void setRange(const double value)  { _range = value; }

private:
    double _center;
    double _range;
};



const bool DeadZone::check(const double value) const
{
    bool valueIsAboveLowBound = value > getLowerBound();
    bool valueIsBelowHighBound = value < getUpperBound();

    bool valueIsWithinDeadZone = valueIsAboveLowBound && valueIsBelowHighBound;

    return valueIsWithinDeadZone;
}

DeadZone::DeadZone()
: _center(0.0),
  _range(0.0)
{}

DeadZone::DeadZone(const double center,
                   const double range)
: _center(center),
  _range(range)
{}

#endif // DEADZONE_H
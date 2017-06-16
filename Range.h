#ifndef RANGE_H
#define RANGE_H

#include "Bounds.h"
#include "CenterAndMagnitude.h"

class Range
{
public:
    Range();
    explicit Range(const Bounds& bounds);
    explicit Range(const CenterAndMagnitude& centerAndMagnitude);

    void enforceRange(double& value);
    const bool checkIfInRange(const double value) const;

    void setCenter(const double value);
    void setMagnitude(const double value);
    void setLowerBound(const double value);
    void setUpperBound(const double value);

    const double getCenter() const         { return _centerAndMagnitude.getCenter(); }
    const double getMagnitude() const      { return _centerAndMagnitude.getMagnitude(); }
    const double getLowerBound() const     { return _bounds.getLowerBound(); }
    const double getUpperBound() const     { return _bounds.getUpperBound(); }

private:
    Bounds _bounds;
    CenterAndMagnitude _centerAndMagnitude;
};



void Range::enforceRange(double& value)
{
    if (value < getLowerBound())
    {
        value = getLowerBound();
        return;
    }

    if (value > getUpperBound())
    {
        value = getUpperBound();
        return;
    }
}

const bool Range::checkIfInRange(const double value) const
{
    bool valueIsAboveLowBound = value > getLowerBound();
    bool valueIsBelowHighBound = value < getUpperBound();

    bool valueIsWithinRange = valueIsAboveLowBound && valueIsBelowHighBound;

    return valueIsWithinRange;
}

void Range::setCenter(const double value)
{
    _centerAndMagnitude.setCenter(value);

    _bounds = Bounds(_centerAndMagnitude);
}

void Range::setMagnitude(const double value)
{
    _centerAndMagnitude.setMagnitude(value);

    _bounds = Bounds(_centerAndMagnitude);
}

void Range::setLowerBound(const double value)
{
    _bounds.setLowerBound(value);

    _centerAndMagnitude = CenterAndMagnitude(_bounds);
}

void Range::setUpperBound(const double value)
{
    _bounds.setUpperBound(value);

    _centerAndMagnitude = CenterAndMagnitude(_bounds);
}

Range::Range()
: _bounds(),
  _centerAndMagnitude()
{}

Range::Range(const Bounds& bounds)
: _bounds(bounds),
  _centerAndMagnitude(bounds)
{}

Range::Range(const CenterAndMagnitude& centerAndMagnitude)
: _bounds(centerAndMagnitude),
  _centerAndMagnitude(centerAndMagnitude)
{}

#endif // RANGE_H
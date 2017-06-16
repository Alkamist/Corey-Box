#ifndef BOUNDS_H
#define BOUNDS_H

class CenterAndMagnitude;

class Bounds
{
public:
    Bounds();
    explicit Bounds(const double bound1, const double bound2);
    explicit Bounds(const CenterAndMagnitude& centerAndMagnitude);

    void setLowerBound(const double lowerBound);
    void setUpperBound(const double upperBound);

    const double getLowerBound() const;
    const double getUpperBound() const;

private:
    double _lowerBound;
    double _upperBound;
};



#include "CenterAndMagnitude.h"

void Bounds::setLowerBound(const double lowerBound)
{
    _lowerBound = lowerBound;

    if (_lowerBound > _upperBound)
        _upperBound = _lowerBound;
}

void Bounds::setUpperBound(const double upperBound)
{
    _upperBound = upperBound;

    if (_upperBound < _lowerBound)
        _lowerBound = _upperBound;
}

const double Bounds::getLowerBound() const
{
    return _lowerBound;
}

const double Bounds::getUpperBound() const
{
    return _upperBound;
}

Bounds::Bounds()
: _lowerBound(0.0),
  _upperBound(0.0)
{}

Bounds::Bounds(const double bound1, const double bound2)
: _lowerBound(0.0),
  _upperBound(0.0)
{
    if (bound1 < bound2)
    {
        setLowerBound(bound1);
        setUpperBound(bound2);
    }
    if (bound1 >= bound2)
    {
        setLowerBound(bound2);
        setUpperBound(bound1);
    }
}

Bounds::Bounds(const CenterAndMagnitude& centerAndMagnitude)
: _lowerBound(0.0),
  _upperBound(0.0)
{
    setLowerBound(centerAndMagnitude.getCenter() - centerAndMagnitude.getMagnitude() / 2.0);
    setUpperBound(centerAndMagnitude.getCenter() + centerAndMagnitude.getMagnitude() / 2.0);
}

#endif // BOUNDS_H
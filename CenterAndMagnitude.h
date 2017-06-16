#ifndef CENTERANDMAGNITUDE_H
#define CENTERANDMAGNITUDE_H

class Bounds;

class CenterAndMagnitude
{
public:
    CenterAndMagnitude();
    explicit CenterAndMagnitude(const double center, const double magnitude);
    explicit CenterAndMagnitude(const Bounds& bounds);

    void setCenter(const double center);
    void setMagnitude(const double magnitude);

    const double getCenter() const;
    const double getMagnitude() const;

private:
    double _center;
    double _magnitude;
};



#include "Bounds.h"

void CenterAndMagnitude::setCenter(const double center)
{
    _center = center;
}

void CenterAndMagnitude::setMagnitude(const double magnitude)
{
    if (magnitude < 0.0)
        _magnitude = -magnitude;
    else
        _magnitude = magnitude;
}

const double CenterAndMagnitude::getCenter() const
{
    return _center;
}

const double CenterAndMagnitude::getMagnitude() const
{
    return _magnitude;
}

CenterAndMagnitude::CenterAndMagnitude()
: _center(0.0),
  _magnitude(0.0)
{}

CenterAndMagnitude::CenterAndMagnitude(const double center, const double magnitude)
: _center(center),
  _magnitude(magnitude)
{
    setMagnitude(magnitude);
}

CenterAndMagnitude::CenterAndMagnitude(const Bounds& bounds)
: _center(0.0),
  _magnitude(0.0)
{
    _magnitude = bounds.getUpperBound() - bounds.getLowerBound();
    _center = bounds.getLowerBound() + _magnitude / 2.0;
}

#endif // CENTERANDMAGNITUDE_H
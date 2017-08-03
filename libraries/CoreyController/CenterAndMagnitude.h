#ifndef CENTERANDMAGNITUDE_H
#define CENTERANDMAGNITUDE_H

template <class T>
class Bounds;

// This class represents a center and a magnitude. Use it to set a range.
template <class T>
class CenterAndMagnitude
{
public:
    CenterAndMagnitude();
    explicit CenterAndMagnitude(const T center, const T magnitude);
    explicit CenterAndMagnitude(const Bounds<T>& bounds);

    void setCenter(const T center);
    void setMagnitude(const T magnitude);

    const T getCenter() const;
    const T getMagnitude() const;

private:
    T _center;
    T _magnitude;
};



#include "Bounds.h"

template <typename T>
void CenterAndMagnitude<T>::setCenter(const T center)
{
    _center = center;
}

template <typename T>
void CenterAndMagnitude<T>::setMagnitude(const T magnitude)
{
    if (magnitude < 0)
        _magnitude = -magnitude;
    else
        _magnitude = magnitude;
}

template <typename T>
const T CenterAndMagnitude<T>::getCenter() const
{
    return _center;
}

template <typename T>
const T CenterAndMagnitude<T>::getMagnitude() const
{
    return _magnitude;
}

template <typename T>
CenterAndMagnitude<T>::CenterAndMagnitude()
: _center(0),
  _magnitude(0)
{}

template <typename T>
CenterAndMagnitude<T>::CenterAndMagnitude(const T center, const T magnitude)
: _center(center),
  _magnitude(magnitude)
{
    setMagnitude(magnitude);
}

template <typename T>
CenterAndMagnitude<T>::CenterAndMagnitude(const Bounds<T>& bounds)
: _center(0),
  _magnitude(0)
{
    _magnitude = (bounds.getUpperBound() - bounds.getLowerBound()) / 2.0;
    _center = bounds.getLowerBound() + _magnitude;
}

#endif // CENTERANDMAGNITUDE_H
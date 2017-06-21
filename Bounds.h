#ifndef BOUNDS_H
#define BOUNDS_H

template <class T>
class CenterAndMagnitude;

template <class T>
class Bounds
{
public:
    Bounds();
    explicit Bounds(const T bound1, const T bound2);
    explicit Bounds(const CenterAndMagnitude<T>& centerAndMagnitude);

    void setLowerBound(const T lowerBound);
    void setUpperBound(const T upperBound);

    const T getLowerBound() const;
    const T getUpperBound() const;

private:
    T _lowerBound;
    T _upperBound;
};



#include "CenterAndMagnitude.h"

template <typename T>
void Bounds<T>::setLowerBound(const T lowerBound)
{
    _lowerBound = lowerBound;

    if (_lowerBound > _upperBound)
        _upperBound = _lowerBound;
}

template <typename T>
void Bounds<T>::setUpperBound(const T upperBound)
{
    _upperBound = upperBound;

    if (_upperBound < _lowerBound)
        _lowerBound = _upperBound;
}

template <typename T>
const T Bounds<T>::getLowerBound() const
{
    return _lowerBound;
}

template <typename T>
const T Bounds<T>::getUpperBound() const
{
    return _upperBound;
}

template <typename T>
Bounds<T>::Bounds()
: _lowerBound(0),
  _upperBound(0)
{}

template <typename T>
Bounds<T>::Bounds(const T bound1, const T bound2)
: _lowerBound(0),
  _upperBound(0)
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

template <typename T>
Bounds<T>::Bounds(const CenterAndMagnitude<T>& centerAndMagnitude)
: _lowerBound(0),
  _upperBound(0)
{
    setLowerBound(centerAndMagnitude.getCenter() - centerAndMagnitude.getMagnitude() / 2.0);
    setUpperBound(centerAndMagnitude.getCenter() + centerAndMagnitude.getMagnitude() / 2.0);
}

#endif // BOUNDS_H
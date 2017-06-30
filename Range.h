#ifndef RANGE_H
#define RANGE_H

#include "Bounds.h"
#include "CenterAndMagnitude.h"

// This class represents a range that can be set from either Bounds or
// CenterAndMagnitude.
template <class T>
class Range
{
public:
    Range();
    explicit Range(const Bounds<T>& bounds);
    explicit Range(const CenterAndMagnitude<T>& centerAndMagnitude);

    T enforceRange(const T value) const;
    const bool checkIfInRange(const T value) const;

    void set(const Range<T>& range);
    void set(const Bounds<T>& bounds);
    void set(const CenterAndMagnitude<T>& centerAndMagnitude);
    void setCenter(const T value);
    void setMagnitude(const T value);
    void setLowerBound(const T value);
    void setUpperBound(const T value);

    const Bounds<T>& getBounds() const                         { return _bounds; }
    const CenterAndMagnitude<T>& getCenterAndMagnitude() const { return _centerAndMagnitude; }
    const T getCenter() const                                  { return _centerAndMagnitude.getCenter(); }
    const T getMagnitude() const                               { return _centerAndMagnitude.getMagnitude(); }
    const T getLowerBound() const                              { return _bounds.getLowerBound(); }
    const T getUpperBound() const                              { return _bounds.getUpperBound(); }

private:
    Bounds<T> _bounds;
    CenterAndMagnitude<T> _centerAndMagnitude;
};


template <typename T>
T Range<T>::enforceRange(const T value) const
{
    if (value < getLowerBound())
        return getLowerBound();

    if (value > getUpperBound())
        return getUpperBound();

    return value;
}

template <typename T>
const bool Range<T>::checkIfInRange(const T value) const
{
    bool valueIsAboveLowBound = value > getLowerBound();
    bool valueIsBelowHighBound = value < getUpperBound();

    bool valueIsWithinRange = valueIsAboveLowBound && valueIsBelowHighBound;

    return valueIsWithinRange;
}

template <typename T>
void Range<T>::set(const Range<T>& range)
{
    *this = range;
}

template <typename T>
void Range<T>::set(const Bounds<T>& bounds)
{
    _bounds = bounds;
}

template <typename T>
void Range<T>::set(const CenterAndMagnitude<T>& centerAndMagnitude)
{
    _centerAndMagnitude = centerAndMagnitude;
}

template <typename T>
void Range<T>::setCenter(const T value)
{
    _centerAndMagnitude.setCenter(value);

    _bounds = Bounds<T>(_centerAndMagnitude);
}

template <typename T>
void Range<T>::setMagnitude(const T value)
{
    _centerAndMagnitude.setMagnitude(value);

    _bounds = Bounds<T>(_centerAndMagnitude);
}

template <typename T>
void Range<T>::setLowerBound(const T value)
{
    _bounds.setLowerBound(value);

    _centerAndMagnitude = CenterAndMagnitude<T>(_bounds);
}

template <typename T>
void Range<T>::setUpperBound(const T value)
{
    _bounds.setUpperBound(value);

    _centerAndMagnitude = CenterAndMagnitude<T>(_bounds);
}

template <typename T>
Range<T>::Range()
: _bounds(),
  _centerAndMagnitude()
{}

template <typename T>
Range<T>::Range(const Bounds<T>& bounds)
: _bounds(bounds),
  _centerAndMagnitude(bounds)
{}

template <typename T>
Range<T>::Range(const CenterAndMagnitude<T>& centerAndMagnitude)
: _bounds(centerAndMagnitude),
  _centerAndMagnitude(centerAndMagnitude)
{}

#endif // RANGE_H
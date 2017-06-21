#ifndef RANGEDVALUE_H
#define RANGEDVALUE_H

#include "Range.h"
#include "PrimitiveMimic.h"

template <class T>
class RangedValue : public PrimitiveMimic<T>
{
public:
    explicit RangedValue(const T value)
    : PrimitiveMimic<T>(value),
      _range(CenterAndMagnitude<T>(value, 0.0))
    {}

    explicit RangedValue(const Range<T>& range)
    : PrimitiveMimic<T>(range.getLowerBound()),
      _range(range)
    {}

    explicit RangedValue(const Bounds<T>& bounds)
    : PrimitiveMimic<T>(bounds.getLowerBound()),
      _range(bounds)
    {}

    explicit RangedValue(const CenterAndMagnitude<T>& centerAndMagnitude)
    : PrimitiveMimic<T>(centerAndMagnitude.getCenter()),
      _range(centerAndMagnitude)
    {}

    virtual const T getValue() const                               { return _range.enforceRange(PrimitiveMimic<T>::getValue()); }

    void setRange(const Range<T>& range)                           { _range = range; }
    void setRange(const Bounds<T>& bounds)                         { _range = bounds; }
    void setRange(const CenterAndMagnitude<T>& centerAndMagnitude) { _range = centerAndMagnitude; }

    const Range<T>& getRange() const                               { return _range; }
    const Bounds<T>& getBounds() const                             { return _range.getBounds(); }
    const CenterAndMagnitude<T>& getCenterAndMagnitude() const     { return _range.getCenterAndMagnitude(); }

    virtual const T operator =(const T value)                      { return PrimitiveMimic<T>::setValue(value); }

private:
    Range<T> _range;
};

#endif // RANGEDVALUE_H
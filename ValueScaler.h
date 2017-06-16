#ifndef VALUESCALER_H
#define VALUESCALER_H

#include "Range.h"

class ValueScaler
{
public:
    explicit ValueScaler(const Range& oldRange, const Range& newRange);

    const double rescaleValue(double value);

private:
    Range _oldRange;
    Range _newRange;
};



const double ValueScaler::rescaleValue(double value)
{
    double scaleFactor = _newRange.getMagnitude() / _oldRange.getMagnitude();
    double zeroBottomedValue = value - _oldRange.getLowerBound();

    return zeroBottomedValue * scaleFactor + _newRange.getLowerBound();
}

ValueScaler::ValueScaler(const Range& oldRange, const Range& newRange)
: _oldRange(oldRange),
  _newRange(newRange)
{}

#endif // VALUESCALER_H
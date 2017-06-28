#ifndef RESCALE_H
#define RESCALE_H

#include "Range.h"

template <typename T>
const T rescale(const T value,
                const Range<T>& fromRange,
                const Range<T>& toRange)
{
    double scaleFactor = toRange.getMagnitude() / fromRange.getMagnitude();
    double zeroBottomedValue = value - fromRange.getLowerBound();

    return zeroBottomedValue * scaleFactor + toRange.getLowerBound();
}

#endif // RESCALE_H
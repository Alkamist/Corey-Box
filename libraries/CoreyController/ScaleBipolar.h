#ifndef SCALEBIPOLAR_H
#define SCALEBIPOLAR_H

// This function will scale an 8bit integer to a given value offset from
// the center (128). It will assume range is 127.
const uint8_t scaleBipolar(const uint8_t value, const uint8_t scaleMagnitude)
{
    int8_t signedValue = value - 128;
    int8_t scaledValue = (signedValue * scaleMagnitude) / 127;
    return scaledValue + 128;
}

// This function will scale an 8bit integer to a given value offset from
// the center (128). You can pass in the reference range.
const uint8_t scaleBipolar(const uint8_t value, const uint8_t scaleMagnitude, const uint8_t fromRange)
{
    int8_t signedValue = value - 128;
    int8_t scaledValue = (signedValue * scaleMagnitude) / fromRange;
    return scaledValue + 128;
}

#endif // SCALEBIPOLAR_H

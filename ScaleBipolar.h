#ifndef SCALEBIPOLAR_H
#define SCALEBIPOLAR_H

const uint8_t scaleBipolar(const uint8_t value, const uint8_t scaleMagnitude)
{
    int8_t signedValue = value - 128;
    int8_t scaledValue = (signedValue * scaleMagnitude) / 127;
    return scaledValue + 128;
}

#endif // SCALEBIPOLAR_H

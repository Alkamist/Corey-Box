#ifndef SCALEBIPOLAR_H
#define SCALEBIPOLAR_H

const uint8_t scaleBipolar(const uint8_t value, const uint8_t scaleMagnitude)
{
    float scaleFactor = scaleMagnitude / 127.0;

    int8_t signedValue = value - 128;
    int8_t scaledValue = signedValue * scaleFactor;
    return scaledValue + 128;
}

#endif // SCALEBIPOLAR_H

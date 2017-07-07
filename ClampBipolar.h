#ifndef CLAMPBIPOLAR_H
#define CLAMPBIPOLAR_H

const uint8_t clampBipolar(const uint8_t value, const uint8_t clampValue)
{
    uint8_t lowClamp = 128 - clampValue;
    uint8_t highClamp = 128 + clampValue;

    if (value < lowClamp)
        return lowClamp;

    if (value > highClamp)
        return highClamp;

    return value;
}

#endif // CLAMPBIPOLAR_H

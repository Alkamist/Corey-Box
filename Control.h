#ifndef CONTROL_H
#define CONTROL_H

class Control
{
public:
    Control()
    : _currentValue(0),
      _previousValue(0)
    {}

    explicit Control(const uint8_t value)
    : _currentValue(value),
      _previousValue(value)
    {}

    virtual void endCycle()                   { _previousValue = _currentValue; }

    const uint8_t getPreviousValue() const    { return _previousValue; }

    const bool justChanged() const            { return _currentValue != _previousValue; }

    const uint8_t getBipolarMagnitude() const
    {
        if (_currentValue >= 128)
            return _currentValue - 128;

        if (_currentValue < 128)
        {
            if (_currentValue == 0)
                return 127;

            return 128 - _currentValue;
        }

        return 0;
    }

    const bool justCrossedCenter() const
    {
        if (_currentValue >= 128 && _previousValue < 128)
            return true;

        if (_currentValue < 128 && _previousValue >= 128)
            return true;

        return false;
    }

    //=================== OPERATORS ===================

    operator uint8_t() const                 { return _currentValue; }

    Control& operator=(const Control& value) { _currentValue = value._currentValue; return *this; }
    Control& operator=(const uint8_t value)  { _currentValue = value; return *this; }

    Control& operator+=(const uint8_t value) { _currentValue += value; return *this; }
    Control& operator-=(const uint8_t value) { _currentValue -= value; return *this; }
    Control& operator*=(const uint8_t value) { _currentValue *= value; return *this; }
    Control& operator/=(const uint8_t value) { _currentValue /= value; return *this; }

    Control operator+(const uint8_t value)   { Control output(*this); output._currentValue += value; return output; }
    Control operator-(const uint8_t value)   { Control output(*this); output._currentValue -= value; return output; }
    Control operator*(const uint8_t value)   { Control output(*this); output._currentValue *= value; return output; }
    Control operator/(const uint8_t value)   { Control output(*this); output._currentValue /= value; return output; }

    Control& operator+=(const float value)   { _currentValue += value; return *this; }
    Control& operator-=(const float value)   { _currentValue -= value; return *this; }
    Control& operator*=(const float value)   { _currentValue *= value; return *this; }
    Control& operator/=(const float value)   { _currentValue /= value; return *this; }

    Control operator+(const float value)     { Control output(*this); output._currentValue += value; return output; }
    Control operator-(const float value)     { Control output(*this); output._currentValue -= value; return output; }
    Control operator*(const float value)     { Control output(*this); output._currentValue *= value; return output; }
    Control operator/(const float value)     { Control output(*this); output._currentValue /= value; return output; }

private:
    uint8_t _currentValue;
    uint8_t _previousValue;
};

#endif // CONTROL_H

#ifndef CONTROL_H
#define CONTROL_H

// This class is an 8bit integer that keeps track of its value last clock cycle.
// It can tell you if it has changed, along with other useful traits.
class Control
{
public:
    Control()
    : _currentValue(0),
      _previousValue(0),
      _justCreated(true)
    {}

    explicit Control(const uint8_t value)
    : _currentValue(value),
      _previousValue(value),
      _justCreated(true)
    {}

    virtual void endCycle()                   { _previousValue = _currentValue; _justCreated = false; }

    const uint8_t getPreviousValue() const    { return _previousValue; }

    const bool justChanged() const            { return (_currentValue != _previousValue) || _justCreated; }

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

    Control& operator+=(const int8_t value)  { _currentValue += value; return *this; }
    Control& operator-=(const int8_t value)  { _currentValue -= value; return *this; }
    Control& operator*=(const int8_t value)  { _currentValue *= value; return *this; }
    Control& operator/=(const int8_t value)  { _currentValue /= value; return *this; }

    Control operator+(const int8_t value)    { Control output(*this); output._currentValue += value; return output; }
    Control operator-(const int8_t value)    { Control output(*this); output._currentValue -= value; return output; }
    Control operator*(const int8_t value)    { Control output(*this); output._currentValue *= value; return output; }
    Control operator/(const int8_t value)    { Control output(*this); output._currentValue /= value; return output; }

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

    bool _justCreated;
};

#endif // CONTROL_H

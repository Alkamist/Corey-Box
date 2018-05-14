#ifndef CONTROL_H
#define CONTROL_H

// This class is an 8bit integer that keeps track of its value last cycle.
// It can tell you if it has changed, along with other useful traits.
class Control
{
public:
    Control()
    : _range(127),
      _currentValue(0),
      _previousValue(0),
      _justCreated(true)
    {}

    explicit Control(const uint8_t value)
    : _range(127),
      _currentValue(value),
      _previousValue(value),
      _justCreated(true)
    {}

    Control(const uint8_t value, const uint8_t range)
    : _range(range),
      _currentValue(value),
      _previousValue(value),
      _justCreated(true)
    {}

    virtual void endCycle()                   { _previousValue = _currentValue; if (_justCreated) _justCreated = false; }

    const uint8_t getValue() const            { return _currentValue; }
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

    const uint8_t getPreviousBipolarMagnitude() const
    {
        if (_previousValue >= 128)
            return _previousValue - 128;

        if (_previousValue < 128)
        {
            if (_previousValue == 0)
                return 127;

            return 128 - _previousValue;
        }

        return 0;
    }

    const bool justLeftCenter() const
    {
        if (((_currentValue > 128) || (_currentValue < 128)) && _previousValue == 128)
            return true;

        return false;
    }

    const bool justCrossedCenter() const
    {
        if (_currentValue > 128 && _previousValue < 128)
            return true;

        if (_currentValue < 128 && _previousValue > 128)
            return true;

        return false;
    }

    void setRange(const uint8_t value)  { _range = value; }
    void setToMinimum()                 { _currentValue = 128 - _range; }
    void setToMaximum()                 { _currentValue = 128 + _range; }
    void setToCenter()                  { _currentValue = 128; }

    void setBipolarMagnitude(const uint8_t value)
    {
        if (_currentValue > 128)
        {
            _currentValue = 128 + value;
        }
        else if (_currentValue < 128)
        {
            _currentValue = 128 - value;
        }
    }

    void scaleBipolarMagnitude(const uint8_t scale)
    {
        int8_t signedValue = _currentValue - 128;
        int8_t scaledValue = (signedValue * scale) / 127;
        _currentValue = scaledValue + 128;
    }

    void clampBipolarMagnitude(const uint8_t clamp)
    {
        uint8_t lowClamp = 128 - clamp;
        uint8_t highClamp = 128 + clamp;

        if (_currentValue < lowClamp)
            _currentValue = lowClamp;

        if (_currentValue > highClamp)
            _currentValue = highClamp;
    }

    //=================== OPERATORS ===================

    operator uint8_t() const                 { return _currentValue; }

    Control& operator=(const Control& value) { _currentValue = value._currentValue; return *this; }
    Control& operator=(const uint8_t value)  { _currentValue = value; return *this; }

private:
    uint8_t _range;

    uint8_t _currentValue;
    uint8_t _previousValue;

    bool _justCreated;
};

#endif // CONTROL_H

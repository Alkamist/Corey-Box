#ifndef DIGIPOT_H
#define DIGIPOT_H

class DigiPot
{
public:
    DigiPot (unsigned int inputResolution, unsigned int inputPin);

    void init();
    void endLoop();

    inline void setRestPosition (double inputDecimal) { _restPosition = getValueByDecimal (inputDecimal); }
    inline void setMinimum (double inputDecimal) { _minimum = getValueByDecimal (inputDecimal); }
    inline void setMaximum (double inputDecimal) { _maximum = getValueByDecimal (inputDecimal); }
    void setRange (double inputDecimal);
    void setCurrentValue (double inputDecimal);

    inline void setMaximumUpdateSpeed (unsigned int inputUpdateSpeed) { _maximumUpdateSpeed = inputUpdateSpeed; }

    inline unsigned int getResolution() { return _resolution; }
    inline unsigned int getRestPosition() { return _restPosition; }
    inline unsigned int getMinimum() { return _minimum; }
    inline unsigned int getMaximum() { return _maximum; }
    inline double getCurrentValue() { return _currentValue; }
private:
    unsigned int _resolution;
    unsigned int _restPosition;
    unsigned int _minimum;
    unsigned int _maximum;
    unsigned int _currentValue;
    unsigned int _previousValue;

    unsigned int _maximumUpdateSpeed;
    elapsedMillis _timeCounter;

    bool _hasChanged;

    unsigned int _slaveSelectPin;
    const byte _writeAddress = 0x00;

    void writeCurrentValue();
    inline void setPreviousValue (double inputDecimal) { _previousValue = getValueByDecimal (inputDecimal); }
    unsigned int getValueByDecimal (double inputDecimal);
};



DigiPot::DigiPot (unsigned int inputResolution, unsigned int inputPin)
 : _resolution (inputResolution),
   _slaveSelectPin (inputPin),
   _hasChanged (false)
{
    setRange (1.00);
    setRestPosition (0.50);
    setCurrentValue (0.50);
    setPreviousValue (0.50);
    setMaximumUpdateSpeed (1);
}

void DigiPot::init()
{
    writeCurrentValue();
}

void DigiPot::endLoop()
{
    if (_currentValue != _previousValue)
        _hasChanged = true;

    if (_hasChanged
     && _timeCounter >= _maximumUpdateSpeed)
    {
        writeCurrentValue();
        _timeCounter = 0;
        _hasChanged = false;
    }

    _previousValue = _currentValue;
}

void DigiPot::writeCurrentValue()
{
    digitalWrite (_slaveSelectPin, LOW);

    SPI.transfer (_writeAddress);
    SPI.transfer (_currentValue);

    digitalWrite (_slaveSelectPin, HIGH);
}

void DigiPot::setRange (double inputDecimal)
{
    if (inputDecimal > 1.00)
        inputDecimal = 1.00;
    if (inputDecimal < 0.50)
        inputDecimal = 0.50;

    double minimumDecimal = 1.00 - inputDecimal;

    _minimum = getValueByDecimal (minimumDecimal);
    _maximum = getValueByDecimal (inputDecimal);
}

void DigiPot::setCurrentValue (double inputDecimal)
{
    unsigned int newCurrentValue = getValueByDecimal (inputDecimal);

    if (newCurrentValue > _maximum)
        newCurrentValue = _maximum;
    if (newCurrentValue < _minimum)
        newCurrentValue = _minimum;

    _currentValue = newCurrentValue;
}

unsigned int DigiPot::getValueByDecimal (double inputDecimal)
{
    // Calculate the output value and round to an int.
    // The + 0.5 at the end ensures proper rounding
    // of positive numbers. Since we are using
    // an unsigned int that's ok.
    return _resolution * inputDecimal - 1 + 0.5;
}

#endif // DIGIPOT_H
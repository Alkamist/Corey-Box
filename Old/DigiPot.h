#ifndef DIGIPOT_H
#define DIGIPOT_H

// This class represents a digital potentiometer
class DigiPot
{
public:
    DigiPot()
     : _resolution(128),
       _minimumValue(0),
       _maximumValue(128),
       _currentValue(63),
       _previousValue(63),
       _maximumUpdateSpeed(1),
       _slaveSelectPin(0)
    {}

    void update();
    void init();

    void setValue(double value);
    void setValueBipolar(double value);

    inline void setResolution(unsigned int resolution)         { _resolution = resolution; }
    inline void setSlaveSelectPin(unsigned int slaveSelectPin) { _slaveSelectPin = slaveSelectPin; }
    inline void setMinimumValue(double minimumValue)           { _minimumValue = minimumValue; }
    inline void setMaximumValue(double maximumValue)           { _maximumValue = maximumValue; }
    inline void setMaximumUpdateSpeed(double speed)            { _maximumUpdateSpeed = speed; }

private:
    unsigned int _resolution;
    double _minimumValue;
    double _maximumValue;
    double _currentValue;
    double _previousValue;

    unsigned int _maximumUpdateSpeed;
    elapsedMillis _timeCounter;

    unsigned int _slaveSelectPin;
    const byte _writeAddress = 0x00;

    void writeCurrentValue();
    unsigned int getValueByDecimal(double decimal);
};



void DigiPot::update()
{
    // Ensure that we aren't writing to the pot faster than
    // the maximum update speed
    if (_timeCounter >= _maximumUpdateSpeed
     && _currentValue != _previousValue)
    {
        writeCurrentValue();
        _timeCounter = 0;
    }

    _previousValue = _currentValue;
}

void init::update()
{
    writeCurrentValue();
}

void DigiPot::setValue(double value)
{
    _currentValue = value;
}

void DigiPot::setValueBipolar(double value)
{
    double adjustedValue = (value + 1.0) / 2.0;

    _currentValue = adjustedValue;
}

void DigiPot::writeCurrentValue()
{
    unsigned int scaledValue = _currentValue * (_maximumValue - _minimumValue) + _minimumValue;
    unsigned int outputValue = getValueByDecimal(scaledValue);

    digitalWrite(_slaveSelectPin, LOW);

    SPI.transfer(_writeAddress);
    SPI.transfer(outputValue);

    digitalWrite(_slaveSelectPin, HIGH);
}

// Calculate the output value and round to an int.
// The + 0.5 at the end ensures proper rounding
// of positive numbers. Since we are using
// an unsigned int that's ok.
unsigned int DigiPot::getValueByDecimal(double decimal)
{
    return _resolution * decimal - 1 + 0.5;
}

#endif // DIGIPOT_H
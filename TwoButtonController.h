#ifndef TWOBUTTONCONTROLLER_H
#define TWOBUTTONCONTROLLER_H

class TwoButtonController
{
public:
    TwoButtonController (unsigned int lowPin, unsigned int highPin);

    void processCurrentValue();

    inline double getCurrentValue() { return _currentValue; }

    inline bool hasChanged() { return _hasChanged; }
private:
    double _currentValue;
    double _previousValue;
    bool _hasChanged;

    unsigned int _lowPin;
    unsigned int _highPin;

    // Button low/high press-order state:
    // 0 means low was pressed first.
    // 1 means high was pressed first.
    // -1 means neither was pressed first.
    int _pressOrder = -1;
};



TwoButtonController::TwoButtonController (unsigned int lowPin, unsigned int highPin)
 : _lowPin (lowPin),
   _highPin (highPin),
   _hasChanged (false)
{}

void TwoButtonController::processCurrentValue()
{
    bool lowIsPressed = !digitalRead (_lowPin);
    bool highIsPressed = !digitalRead (_highPin);
    bool lowWasPressedFirst = lowIsPressed && (_pressOrder == 0);
    bool highWasPressedFirst = highIsPressed && (_pressOrder == 1);

    // Low Cases
    if (lowIsPressed && highWasPressedFirst)
    {
        _pressOrder = 1;
        _currentValue = 0.0;
    }
    else if (lowIsPressed && !highIsPressed)
    {
        _pressOrder = 0;
        _currentValue = 0.0;
    }

    // High Cases
    else if (lowWasPressedFirst && highIsPressed)
    {
        _pressOrder = 0;
        _currentValue = 1.00;
    }
    else if (!lowIsPressed && highIsPressed)
    {
        _pressOrder = 1;
        _currentValue = 1.00;
    }

    // Middle Cases
    else if (!lowIsPressed && !highIsPressed)
    {
        _pressOrder = -1;
        _currentValue = 0.50;
    }
    else if (lowIsPressed && highIsPressed && (_pressOrder == -1))
    {
        _pressOrder = -1;
        _currentValue = 0.50;
    }

    if (_currentValue != _previousValue)
        _hasChanged = true;
    else
        _hasChanged = false;

    _previousValue = _currentValue;
}

#endif // TWOBUTTONCONTROLLER_H
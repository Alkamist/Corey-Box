#ifndef ANALOGBUTTON_H
#define ANALOGBUTTON_H

// This class is an arbitrary implementation of an
// analog button.
class AnalogButton
{
public:
    AnalogButton()
     : _hasChanged(false),
       _pressed(false),
       _justPressed(false),
       _justReleased(false),
       _currentValue(0.0),
       _previousValue(0.0),
       _deadZone(0.0)
    {}

    void update();

    void setValue(double value);
    void setDeadZone(double deadZone);

    inline bool hasChanged()        { return _hasChanged; }

    inline bool pressed()           { return _pressed; }
    inline bool justPressed()       { return _justPressed; }
    inline bool justReleased()      { return _justReleased; }

    inline double getValue()        { return _currentValue; }
    inline elapsedMillis getTimer() { return _timer; }

private:
    double _currentValue;
    double _previousValue;

    double _deadZone;

    bool _hasChanged;

    bool _pressed;
    bool _justPressed;
    bool _justReleased;

    elapsedMillis _timer;

    void applyDeadZone();
};



void AnalogButton::update()
{
    updateState();
    applyDeadZone();

    _previousValue = _currentValue;
}

void AnalogButton::setValue(double value)
{
    _currentValue = value;
}

void AnalogButton::setDeadZone(double deadZone)
{
    _deadZone = deadZone;
}

void AnalogButton::applyDeadZone()
{
    if (_currentValue < _deadZone)
        _currentValue = 0.0;

    _currentValue = (_currentValue - _deadZone) / (1.0 - _deadZone);
}

void AnalogButton::updateState()
{
    if (_currentValue != _previousValue)
        _hasChanged = true;
    else
        _hasChanged = false;

    if (_currentValue > _deadZone)
    {
        _pressed = true;
        _justReleased = false;

        if (_previousValue <= _deadZone)
        {
            _timer = 0;
            _justPressed = true;
        }
        else
            _justPressed = false;
    }
    else if (_currentValue <= _deadZone)
    {
        _pressed = false;
        _justPressed = false;

        if (_previousValue > _deadZone)
        {
            _timer = 0;
            _justReleased = true;
        }
        else
            _justReleased = false;
    }
}

#endif // ANALOGBUTTON_H
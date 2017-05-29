#ifndef SIMPLEBUTTON_H
#define SIMPLEBUTTON_H

class SimpleButton
{
public:
    SimpleButton (unsigned int pin);

    void update();

    inline bool isHeldDown()      { return _isHeldDown; }
    inline bool wasJustPressed()  { return _wasJustPressed; }
    inline bool wasJustReleased() { return _wasJustReleased; }

    inline bool isHeldDownExtra()      { return _isHeldDown; }
    inline bool wasJustReleasedExtra() { return _wasJustReleasedExtra; }

    inline void setExtraHoldTime (unsigned int extraTime) { _extraHoldTime = extraTime; }

    inline elapsedMillis getStateTimer() { return _stateTimer; }
private:
    unsigned int _pin;

    bool _isHeldDown;
    bool _wasPreviouslyHeldDown;
    bool _wasJustPressed;
    bool _wasJustReleased;

    bool _isHeldDownExtra;
    bool _wasPreviouslyHeldDownExtra;
    bool _wasJustReleasedExtra;

    elapsedMillis _stateTimer;
    elapsedMillis _extraHoldTimeCounter;
    unsigned int _extraHoldTime;
};



SimpleButton::SimpleButton (unsigned int pin)
 : _pin (pin),
   _wasJustPressed (false),
   _wasJustReleased (false),
   _isHeldDown (false),
   _wasPreviouslyHeldDown (false),
   _extraHoldTime (0)
{}

void SimpleButton::update()
{
    _isHeldDown = !digitalRead (_pin);

    if (_isHeldDown)
    {
        _wasJustReleased = false;

        if (_wasPreviouslyHeldDown)
            _wasJustPressed = false;
        else
            _wasJustPressed = true;
    }
    else
    {
        _wasJustPressed = false;

        if (_wasPreviouslyHeldDown)
            _wasJustReleased = true;
        else
            _wasJustReleased = false;
    }

    if (_wasJustPressed || _wasJustReleased)
        _stateTimer = 0;

    _wasPreviouslyHeldDown = _isHeldDown;

    // =============== Extra Time ===============

    if (_wasJustReleased)
    {
        _isHeldDownExtra = true;
        _extraHoldTimeCounter = 0;
    }

    if (_extraHoldTimeCounter > _extraHoldTime)
        _isHeldDownExtra = false;

    if ((_isHeldDownExtra == false)
     && (_wasPreviouslyHeldDownExtra == true))
        _wasJustReleasedExtra = true;
    else
        _wasJustReleasedExtra = false;

    _wasPreviouslyHeldDownExtra = _isHeldDownExtra;
}

#endif // SIMPLEBUTTON_H
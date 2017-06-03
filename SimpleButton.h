#ifndef SIMPLEBUTTON_H
#define SIMPLEBUTTON_H

#include "UpdatedBool.h"

// This class represents a simple button that can be manipulated
// by using the setValue() method.
class SimpleButton
{
public:
    void update();

    inline void setValue(bool value) { _pressed.set(value); }

    inline bool pressed()            { return _pressed.isTrue(); }
    inline bool justPressed()        { return _justPressed.isTrue(); }
    inline bool justReleased()       { return _justReleased.isTrue(); }

    inline void resetTimer()         { _timer = 0; }
    inline elapsedMillis getTimer()  { return _timer; }

private:
    UpdatedBool _pressed;
    UpdatedBool _justPressed;
    UpdatedBool _justReleased;

    elapsedMillis _timer;
};



void SimpleButton::update()
{
    if (_pressed.hasChanged())
    {
        resetTimer();

        if (_pressed.isTrue())
            _justPressed.set(true);
        else
            _justReleased.set(true);
    }
    else
    {
        _justPressed.set(false);
        _justReleased.set(false);
    }

    _pressed.update();
    _justPressed.update();
    _justReleased.update();
}

#endif // SIMPLEBUTTON_H
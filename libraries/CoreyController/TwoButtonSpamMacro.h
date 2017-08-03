#ifndef TWOBUTTONSPAMMACRO_H
#define TWOBUTTONSPAMMACRO_H

#include "SpamMacro.h"
#include "Frames.h"

// This class is a macro that will spam two buttons alternating
// every frame.
class TwoButtonSpamMacro
{
public:
    TwoButtonSpamMacro()
    {
        initMacro();
    }

    void process();
    void endCycle();

    const bool isRunning() const               { return _button1.isRunning() || _button2.isRunning(); }

    const ActivatorMacro& getButton1() const   { return _button1; }
    const ActivatorMacro& getButton2() const   { return _button2; }

    virtual TwoButtonSpamMacro& operator=(const bool value)
    {
        _button1 = value;
        _button2 = value;

        return *this;
    }

private:
    SpamMacro _button1;
    SpamMacro _button2;

    void initMacro();
};



void TwoButtonSpamMacro::process()
{
    _button1.process();
    _button2.process();
}

void TwoButtonSpamMacro::endCycle()
{
    _button1.endCycle();
    _button2.endCycle();
}

void TwoButtonSpamMacro::initMacro()
{
    _button2.setStartDelay(frames(1));
}

#endif // TWOBUTTONSPAMMACRO_H

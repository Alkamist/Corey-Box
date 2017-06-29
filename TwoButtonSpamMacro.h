#ifndef TWOBUTTONSPAMMACRO_H
#define TWOBUTTONSPAMMACRO_H

#include "SpamMacro.h"
#include "Frames.h"

class TwoButtonSpamMacro
{
public:
    TwoButtonSpamMacro()
    {
        initMacro();
    }

    void process();
    void endCycle();

    void setActivatorState(const bool state)
    {
        _button1.setActivatorState(state);
        _button2.setActivatorState(state);
    }

    const bool isRunning() const               { return _button1.isRunning() || _button2.isRunning(); }

    const ActivatorMacro& getButton1() const   { return _button1; }
    const ActivatorMacro& getButton2() const   { return _button2; }

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
    _button2.setStartDelay(frames(1.0));
}

#endif // TWOBUTTONSPAMMACRO_H

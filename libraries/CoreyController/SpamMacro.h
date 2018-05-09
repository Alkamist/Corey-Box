#ifndef SPAMMACRO_H
#define SPAMMACRO_H

#include "ActivatorMacro.h"

// This macro simply spams an input alternating on and off every 3 half frames.
// If you set it any faster then it will eat inputs on console.
class SpamMacro : public ActivatorMacro
{
public:
    SpamMacro()
    : ActivatorMacro()
    {
        initMacro();
        setLooping(true);
    }

    virtual SpamMacro& operator=(const bool value) { ActivatorMacro::operator=(value); return *this; }

private:
    void initMacro();
};



void SpamMacro::initMacro()
{
    ActivatorMacro::clearMacro();

    ActivatorMacro::addInput(ActivatorMacroUnit(true, 3));
    ActivatorMacro::addInput(ActivatorMacroUnit(false, 3));
}

#endif // SPAMMACRO_H
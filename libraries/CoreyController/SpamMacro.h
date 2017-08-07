#ifndef SPAMMACRO_H
#define SPAMMACRO_H

#include "ActivatorMacro.h"

// This macro simply spams an input alternating on and off every frame.
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

    // Jump
    ActivatorMacro::addInput(ActivatorMacroUnit(true, 1));
    ActivatorMacro::addInput(ActivatorMacroUnit(false, 1));
}

#endif // SPAMMACRO_H
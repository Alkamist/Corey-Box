#ifndef SPAMMACRO_H
#define SPAMMACRO_H

#include "ActivatorMacro.h"
#include "Frames.h"

class SpamMacro : public ActivatorMacro
{
public:
    SpamMacro()
    : ActivatorMacro()
    {
        initMacro();
        setLooping(true);
    }

private:
    void initMacro();
};



void SpamMacro::initMacro()
{
    ActivatorMacro::clearMacro();

    // Jump
    ActivatorMacro::addInput(ActivatorMacroUnit(true, frames(1.0)));
    ActivatorMacro::addInput(ActivatorMacroUnit(false, frames(1.0)));
}

#endif // SPAMMACRO_H
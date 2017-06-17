#ifndef TESTMACRO_H
#define TESTMACRO_H

#include "ControlMacro.h"
#include "Frames.h"

class TestMacro : public ControlMacro
{
public:
    explicit TestMacro(const ControlValue& activator);

    virtual void update();

private:
};



void TestMacro::update()
{
    ControlMacro::update();
}

TestMacro::TestMacro(const ControlValue& activator)
: ControlMacro(activator)
{
    addInput(ControlMacroUnit(true, Frames(30, 60).getMillis()));
    addInput(ControlMacroUnit(false, Frames(30, 60).getMillis()));
    addInput(ControlMacroUnit(true, Frames(30, 60).getMillis()));
    addInput(ControlMacroUnit(false, Frames(30, 60).getMillis()));
}

#endif // TESTMACRO_H
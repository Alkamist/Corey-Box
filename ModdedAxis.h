#ifndef MODDEDAXIS_H
#define MODDEDAXIS_H

#include "AxisModifier.h"
#include "ControlValue.h"
#include "Array.h"

class ModdedAxis : public ControlValue
{
public:
    ModdedAxis();
    ModdedAxis(ControlValue& controller);

    void update();

    void setController(ControlValue& controller) { _controller = &controller; }
    void addModifier(AxisModifier& modifier)     { _mods.insertAtEnd(&modifier); }

private:
    ControlValue* _controller;

    Array<AxisModifier*> _mods;

    void handleModifiers();
};



void ModdedAxis::update()
{
    ControlValue::update();

    if (_controller != nullptr)
        setValue(_controller->getValue());

    if (_mods.getLength() > 0)
        handleModifiers();
}

void ModdedAxis::handleModifiers()
{
    for (int i = 0; i < _mods.getLength(); ++i)
    {
        if (_mods[i]->getActivator().isActive())
        {
            _mods[i]->applyModifier(*this);
            break;
        }
    }
}

ModdedAxis::ModdedAxis()
: ControlValue(0.5, 0.5),
  _controller(nullptr)
{}

ModdedAxis::ModdedAxis(ControlValue& controller)
: ControlValue(0.5, 0.5),
  _controller(&controller)
{}

#endif // MODDEDAXIS_H
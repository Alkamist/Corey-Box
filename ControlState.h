#ifndef CONTROLSTATE_H
#define CONTROLSTATE_H

#include "UpdatedValue.h"

class ControlState : public UpdatedValue<bool>
{
public:
    ControlState()
    : UpdatedValue<bool>(false)
    {}

    explicit ControlState(const bool value)
    : UpdatedValue<bool>(value)
    {}

    const bool justActivated() const   { return hasChanged() && (getValue() == true); }
    const bool justDeactivated() const { return hasChanged() && (getValue() == false); }
};

#endif // CONTROLSTATE_H
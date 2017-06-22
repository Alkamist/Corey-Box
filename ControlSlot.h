#ifndef CONTROLSLOT_H
#define CONTROLSLOT_H

#include "Control.h"

class ControlSlot
{
public:
    ControlSlot()
    : _control(nullptr)
    {}

    explicit ControlSlot(const Control& control)
    : _control(&control)
    {}

    void setControl(const Control& control)    { _control = &control; }

    const Control& getControl() const          { return *_control; }

    const double getValue() const              { return _control->getValue(); }

    const Range<double>& getValueRange() const { return _control->getValueRange(); }

    const bool hasChanged() const              { return _control->hasChanged(); }
    const bool stateHasChanged() const         { return _control->stateHasChanged(); }
    const bool isActive() const                { return _control->isActive(); }
    const bool justActivated() const           { return _control->justActivated(); }
    const bool justDeactivated() const         { return _control->justDeactivated(); }

    const unsigned int getStateTime() const    { return _control->getStateTime(); }

private:
    const Control* _control;
};

#endif // CONTROLSLOT_H
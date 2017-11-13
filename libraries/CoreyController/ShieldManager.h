#ifndef SHIELDMANAGER_H
#define SHIELDMANAGER_H

#include "Activator.h"
#include "ToggleActivator.h"
#include "TwoButtonStateTracker.h"

// This class manages toggling shield and lightshield.
class ShieldManager : public Activator
{
public:
    ShieldManager()
    : Activator(),
      _lightShieldValue(80)
    {}

    void process()
    {
        *this = _activator;

        _toggleTracker.setState1(_activator);
        _toggleTracker.setState2(_toggleState);
        _toggleTracker.process();

        _isLightShield = _activator && _toggleTracker.state1WasFirst() && _toggleState.justActivated();
        _isLightShield.process();

        if (!_activator)
            _isLightShield.setState(false);
    }

    void endCycle()
    {
        _activator.endCycle();
        _isLightShield.endCycle();
        _toggleState.endCycle();
    }

    const bool getHardShieldState() const              { return *this && !_isLightShield; }
    const bool getLightShieldState() const             { return *this && _isLightShield; }
    const uint8_t getLightShieldOutput() const
    {
        uint8_t lightShieldOutput = 0;

        if (getLightShieldState())
            lightShieldOutput = _lightShieldValue;

        return lightShieldOutput;
    }

    void setToggleState(const bool state)              { _toggleState = state; }
    void setActivator(const bool activator)            { _activator = activator; }

    virtual ShieldManager& operator=(const bool value) { setState(value); return *this; }

private:
    Activator _activator;
    Activator _toggleState;

    TwoButtonStateTracker _toggleTracker;

    ToggleActivator _isLightShield;

    uint8_t _lightShieldValue;
};

#endif // SHIELDMANAGER_H

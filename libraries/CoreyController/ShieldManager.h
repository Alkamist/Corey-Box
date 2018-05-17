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
        _toggleTracker.setState1(*this);
        _toggleTracker.setState2(_toggleState);
        _toggleTracker.process();

        // Allow light shield when the shield is activated
        if (justActivated())                     _forbidLightShield = false;
        // But disable light shield if jump is activated while in shield
        if (*this && (_jumpState.justActivated() || _shieldDropState.justActivated())) _forbidLightShield = true;

        _isLightShield = *this && _toggleTracker.state1WasFirst() && _toggleState.justActivated() && !_forbidLightShield;
        _isLightShield.process();

        if (!*this)
            _isLightShield.setState(false);
    }

    void endCycle()
    {
        Activator::endCycle();
        _forbidLightShield.endCycle();
        _isLightShield.endCycle();
        _toggleState.endCycle();
        _jumpState.endCycle();
        _shieldDropState.endCycle();
    }

    const bool lightShieldIsAllowed() const            { return !_forbidLightShield; }

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
    void setJumpState(const bool state)                { _jumpState = state; }
    void setShieldDropState(const bool state)          { _shieldDropState = state; }

    virtual ShieldManager& operator=(const bool value) { setState(value); return *this; }

private:
    Activator _forbidLightShield;

    Activator _toggleState;
    Activator _jumpState;
    Activator _shieldDropState;

    TwoButtonStateTracker _toggleTracker;

    ToggleActivator _isLightShield;

    uint8_t _lightShieldValue;
};

#endif // SHIELDMANAGER_H

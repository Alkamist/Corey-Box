#ifndef SHIELDMANAGER_H
#define SHIELDMANAGER_H

#include "Activator.h"
#include "TemporaryActivator.h"
#include "ToggleActivator.h"

// This class manages toggling shield and lightshield. It also holds shield for
// 1 extra frame after release to create a more natural feel on the button only
// controller.
class ShieldManager : public Activator
{
public:
    ShieldManager()
    : Activator(),
      _lightShieldValue(80)
    {
        _shieldExtension.setTime(frames(1));
    }

    void process()
    {
        _shieldExtension = _activator.justDeactivated();
        _shieldExtension.process();

        if (_activator || _shieldExtension)
            *this = true;
        else
            *this = false;

        _isLightShield = _lightShieldActivator.justActivated();
        _isLightShield.process();

        if (!_activator && !_shieldExtension)
            _isLightShield.setState(false);
    }

    void endCycle()
    {
        _activator.endCycle();
        _shieldExtension.endCycle();
        _isLightShield.endCycle();
        _lightShieldActivator.endCycle();
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

    void setLightShieldState(const bool value)         { _lightShieldActivator = value; }
    void setActivator(const bool activator)            { _activator = activator; }

    virtual ShieldManager& operator=(const bool value) { setState(value); return *this; }

private:
    Activator _activator;
    TemporaryActivator _shieldExtension;

    Activator _lightShieldActivator;

    ToggleActivator _isLightShield;

    uint8_t _lightShieldValue;
};

#endif // SHIELDMANAGER_H

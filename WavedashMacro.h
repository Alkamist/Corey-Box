#ifndef WAVEDASHMACRO_H
#define WAVEDASHMACRO_H

#include "ControlMacro.h"
#include "Frames.h"

class WavedashMacro
{
public:
    WavedashMacro(const ControlValue& activator,
                  const ControlValue& trimUp,
                  const ControlValue& trimDown);

    void update();

    const ControlValue& getJump() const     { return _jump; }
    const ControlValue& getAirDodge() const { return _airDodge; }

private:
    const ControlValue* _trimUp;
    const ControlValue* _trimDown;

    ControlMacro _jump;
    ControlMacro _airDodge;

    UpdatedValue<unsigned int> _airDodgeDelayFrames;

    void handleTrim();
    void initMacro();
};



void WavedashMacro::update()
{
    _airDodgeDelayFrames.update();
    _jump.update();
    _airDodge.update();

    handleTrim();
}

void WavedashMacro::handleTrim()
{
    unsigned int minimumDelay = 3;
    unsigned int maximumDelay = 8;

    if (_trimUp->justActivated())
        _airDodgeDelayFrames.setValue(_airDodgeDelayFrames.getValue() + 1);

    if (_trimDown->justActivated())
        _airDodgeDelayFrames.setValue(_airDodgeDelayFrames.getValue() - 1);

    if (_airDodgeDelayFrames.getValue() < minimumDelay)
        _airDodgeDelayFrames.setValue(minimumDelay);

    if (_airDodgeDelayFrames.getValue() > maximumDelay)
        _airDodgeDelayFrames.setValue(maximumDelay);

    if (_airDodgeDelayFrames.hasChanged())
        initMacro();
}

void WavedashMacro::initMacro()
{
    _jump.clearMacro();
    _airDodge.clearMacro();

    Frames airDodgeDelay(_airDodgeDelayFrames.getValue(), 60);

    // Jump
    _jump.addInput(ControlMacroUnit(true, airDodgeDelay.getMillis()));
    _jump.addInput(ControlMacroUnit(false, Frames(10, 60).getMillis()));

    // Air Dodge
    _airDodge.addInput(ControlMacroUnit(false, airDodgeDelay.getMillis()));
    _airDodge.addInput(ControlMacroUnit(true, Frames(9, 60).getMillis()));
    _airDodge.addInput(ControlMacroUnit(false, Frames(1, 60).getMillis()));
}

WavedashMacro::WavedashMacro(const ControlValue& activator,
                             const ControlValue& trimUp,
                             const ControlValue& trimDown)
: _airDodgeDelayFrames(3),
  _jump(activator),
  _airDodge(activator),
  _trimUp(&trimUp),
  _trimDown(&trimDown)
{
    initMacro();
}

#endif // WAVEDASHMACRO_H
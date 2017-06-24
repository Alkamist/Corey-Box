#ifndef WAVEDASHMACRO_H
#define WAVEDASHMACRO_H

#include "ControlMacro.h"
#include "Frames.h"

class WavedashMacro
{
public:
    WavedashMacro()
    : _airDodgeDelayFrames(3.0),
      _airDodgeDelayFramesPrev(3.0),
      _minimumDelay(3.0),
      _maximumDelay(8.0)
    {
        initMacro();
    }

    void update();

    void setControls(const bool activator, const bool trimDown, const bool trimUp);

    const bool isRunning() const           { return _jump.isRunning() || _L.isRunning() || _R.isRunning(); }

    const ControlMacro& getJump() const    { return _jump; }
    const ControlMacro& getL() const       { return _L; }
    const ControlMacro& getR() const       { return _R; }

private:
    ControlState _activator;
    ControlState _trimUp;
    ControlState _trimDown;

    ControlMacro _jump;
    ControlMacro _L;
    ControlMacro _R;

    double _airDodgeDelayFrames;
    double _airDodgeDelayFramesPrev;

    const double _minimumDelay;
    const double _maximumDelay;

    void handleTrim();
    void initMacro();
};



void WavedashMacro::update()
{
    _airDodgeDelayFramesPrev = _airDodgeDelayFrames;
    _activator.update();
    _trimUp.update();
    _trimDown.update();
    _jump.update();
    _L.update();
    _R.update();
}

void WavedashMacro::setControls(const bool activator, const bool trimDown, const bool trimUp)
{
    _activator = activator;
    _trimDown = trimDown;
    _trimUp = trimUp;

    handleTrim();

    if (_airDodgeDelayFrames != _airDodgeDelayFrames)
        initMacro();

    _jump.setControls(_activator);
    _L.setControls(_activator);
    _R.setControls(_activator);
}

void WavedashMacro::initMacro()
{
    _jump.clearMacro();
    _L.clearMacro();
    _R.clearMacro();

    // Jump
    _jump.addInput(ControlMacroUnit(true, frames(2)));
    _jump.addInput(ControlMacroUnit(false, frames(1)));

    // L
    _L.setStartDelay(frames(_airDodgeDelayFrames - 1));
    _L.addInput(ControlMacroUnit(false, frames(1)));
    _L.addInput(ControlMacroUnit(true, frames(1)));
    _L.addInput(ControlMacroUnit(false, frames(1)));

    // R
    _R.setStartDelay(frames(_airDodgeDelayFrames - 2));
    _R.addInput(ControlMacroUnit(false, frames(1)));
    _R.addInput(ControlMacroUnit(true, frames(1)));
    _R.addInput(ControlMacroUnit(false, frames(1)));
    _R.addInput(ControlMacroUnit(true, frames(1)));
}

void WavedashMacro::handleTrim()
{
    if (_trimUp.justActivated())
        ++_airDodgeDelayFrames;

    if (_trimDown.justActivated())
        --_airDodgeDelayFrames;

    if (_airDodgeDelayFrames < _minimumDelay)
        _airDodgeDelayFrames = _minimumDelay;

    if (_airDodgeDelayFrames > _maximumDelay)
        _airDodgeDelayFrames = _maximumDelay;
}

#endif // WAVEDASHMACRO_H
#ifndef WAVEDASHMACRO_H
#define WAVEDASHMACRO_H

#include "ActivatorMacro.h"
#include "Frames.h"

class WavedashMacro
{
public:
    WavedashMacro(const Activator& activator, const Activator& trimDown, const Activator& trimUp)
    : _trimDown(trimDown),
      _trimUp(trimUp),
      _jump(activator),
      _L(activator),
      _R(activator),
      _airDodgeDelayFrames(3.0),
      _airDodgeDelayFramesPrev(3.0),
      _minimumDelay(3.0),
      _maximumDelay(8.0)
    {
        initMacro();
    }

    void process();
    void endCycle();

    const bool isRunning() const          { return _jump.isRunning() || _L.isRunning() || _R.isRunning(); }

    const ActivatorMacro& getJump() const { return _jump; }
    const ActivatorMacro& getL() const    { return _L; }
    const ActivatorMacro& getR() const    { return _R; }

private:
    const Activator& _trimDown;
    const Activator& _trimUp;

    ActivatorMacro _jump;
    ActivatorMacro _L;
    ActivatorMacro _R;

    double _airDodgeDelayFrames;
    double _airDodgeDelayFramesPrev;

    const double _minimumDelay;
    const double _maximumDelay;

    void handleTrim();
    void initMacro();
};



void WavedashMacro::process()
{
    handleTrim();

    if (_airDodgeDelayFrames != _airDodgeDelayFramesPrev)
        initMacro();

    _jump.process();
    _L.process();
    _R.process();
}

void WavedashMacro::endCycle()
{
    _airDodgeDelayFramesPrev = _airDodgeDelayFrames;
    _jump.endCycle();
    _L.endCycle();
    _R.endCycle();
}

void WavedashMacro::initMacro()
{
    _jump.clearMacro();
    _L.clearMacro();
    _R.clearMacro();

    // Jump
    _jump.addInput(ActivatorMacroUnit(true, frames(2.0)));
    _jump.addInput(ActivatorMacroUnit(false, frames(1.0)));

    // L Pure
    //_L.setStartDelay(frames(_airDodgeDelayFrames));
    //_L.addInput(ActivatorMacroUnit(true, frames(2.0)));
    //_L.addInput(ActivatorMacroUnit(false, frames(1.0)));

    // L
    _L.setStartDelay(frames(_airDodgeDelayFrames - 1.0));
    _L.addInput(ActivatorMacroUnit(true, frames(2.0)));
    _L.addInput(ActivatorMacroUnit(false, frames(1.0)));

    // R
    _R.setStartDelay(frames(1.0));
    _R.addInput(ActivatorMacroUnit(false, frames(_airDodgeDelayFrames - 1.0)));
    _R.addInput(ActivatorMacroUnit(true, frames(2.0)));
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
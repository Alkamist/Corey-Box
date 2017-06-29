#ifndef WAVEDASHMACRO_H
#define WAVEDASHMACRO_H

#include "ActivatorMacro.h"
#include "Frames.h"

class WavedashMacro
{
public:
    WavedashMacro()
    : _airDodgeDelayFrames(3.0),
      _minimumDelay(3.0),
      _maximumDelay(8.0)
    {
        initMacro();
    }

    void process();
    void endCycle();

    void setActivatorState(const bool state)
    {
        _jump.setActivatorState(state);
        _L.setActivatorState(state);
        _R.setActivatorState(state);
    }

    void trimDown()
    {
        --_airDodgeDelayFrames;

        if (_airDodgeDelayFrames < _minimumDelay)
            _airDodgeDelayFrames = _minimumDelay;

        initMacro();
    }

    void trimUp()
    {
        ++_airDodgeDelayFrames;

        if (_airDodgeDelayFrames > _maximumDelay)
            _airDodgeDelayFrames = _maximumDelay;

        initMacro();
    }

    const bool isRunning() const            { return _jump.isRunning() || _L.isRunning() || _R.isRunning(); }

    const ActivatorMacro& getJump() const   { return _jump; }
    const ActivatorMacro& getL() const      { return _L; }
    const ActivatorMacro& getR() const      { return _R; }

private:
    ActivatorMacro _jump;
    ActivatorMacro _L;
    ActivatorMacro _R;

    double _airDodgeDelayFrames;

    const double _minimumDelay;
    const double _maximumDelay;

    void initMacro();
};



void WavedashMacro::process()
{
    _jump.process();
    _L.process();
    _R.process();
}

void WavedashMacro::endCycle()
{
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

    // L
    _L.setStartDelay(frames(_airDodgeDelayFrames - 1.0));
    _L.addInput(ActivatorMacroUnit(true, frames(2.0)));
    _L.addInput(ActivatorMacroUnit(false, frames(1.0)));

    // R
    _R.setStartDelay(frames(1.0));
    _R.addInput(ActivatorMacroUnit(false, frames(_airDodgeDelayFrames - 1.0)));
    _R.addInput(ActivatorMacroUnit(true, frames(2.0)));
}

#endif // WAVEDASHMACRO_H

#ifndef WAVEDASHMACRO_H
#define WAVEDASHMACRO_H

#include "ActivatorMacro.h"
#include "Frames.h"

// This class is a wavedash macro. You can trim the delay time to account for
// different characters having different jumpsquats. I was having some timing
// issues in the game so I had to do a trick where you push L and R spaced apart
// by 1 frame. The macro does some wavedashes a frame late but I'm not sure how
// to fix that.
class WavedashMacro
{
public:
    WavedashMacro()
    : _airDodgeDelayFrames(3),
      _minimumDelay(3),
      _maximumDelay(8)
    {
        initMacro();
    }

    void process();
    void endCycle();

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

    const bool isRunning() const          { return _jump.isRunning() || _LMacro.isRunning() || _RMacro.isRunning(); }

    const ActivatorMacro& getJump() const { return _jump; }
    const ActivatorMacro& getL() const    { return _LMacro; }
    const ActivatorMacro& getR() const    { return _RMacro; }

    virtual WavedashMacro& operator=(const bool value)
    {
        _jump = value;
        _LMacro = value;
        _RMacro = value;

        return *this;
    }

private:
    ActivatorMacro _jump;
    ActivatorMacro _LMacro;
    ActivatorMacro _RMacro;

    uint16_t _airDodgeDelayFrames;

    const uint16_t _minimumDelay;
    const uint16_t _maximumDelay;

    void initMacro();
};



void WavedashMacro::process()
{
    _jump.process();
    _LMacro.process();
    _RMacro.process();
}

void WavedashMacro::endCycle()
{
    _jump.endCycle();
    _LMacro.endCycle();
    _RMacro.endCycle();
}

void WavedashMacro::initMacro()
{
    _jump.clearMacro();
    _LMacro.clearMacro();
    _RMacro.clearMacro();

    // Jump
    _jump.addInput(ActivatorMacroUnit(true, frames(2)));
    _jump.addInput(ActivatorMacroUnit(false, frames(1)));

    // L
    _LMacro.setStartDelay(frames(_airDodgeDelayFrames - 1) + 8);
    _LMacro.addInput(ActivatorMacroUnit(true, frames(2)));
    _LMacro.addInput(ActivatorMacroUnit(false, frames(1)));

    // R
    _RMacro.setStartDelay(frames(1) + 8);
    _RMacro.addInput(ActivatorMacroUnit(false, frames(_airDodgeDelayFrames - 1)));
    _RMacro.addInput(ActivatorMacroUnit(true, frames(2)));
}

#endif // WAVEDASHMACRO_H

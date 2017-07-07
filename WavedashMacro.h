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

    const bool isRunning() const          { return _jump.isRunning() || _L.isRunning() || _R.isRunning(); }

    const ActivatorMacro& getJump() const { return _jump; }
    const ActivatorMacro& getL() const    { return _L; }
    const ActivatorMacro& getR() const    { return _R; }

    WavedashMacro& operator =(const bool value)
    {
        _jump = value;
        _L = value;
        _R = value;

        return *this;
    }

private:
    ActivatorMacro _jump;
    ActivatorMacro _L;
    ActivatorMacro _R;

    uint16_t _airDodgeDelayFrames;

    const uint16_t _minimumDelay;
    const uint16_t _maximumDelay;

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
    _jump.addInput(ActivatorMacroUnit(true, frames(2)));
    _jump.addInput(ActivatorMacroUnit(false, frames(1)));

    // L
    _L.setStartDelay(frames(_airDodgeDelayFrames - 1) + 8);
    _L.addInput(ActivatorMacroUnit(true, frames(2)));
    _L.addInput(ActivatorMacroUnit(false, frames(1)));

    // R
    _R.setStartDelay(frames(1));
    _R.addInput(ActivatorMacroUnit(false, frames(_airDodgeDelayFrames - 1) + 8));
    _R.addInput(ActivatorMacroUnit(true, frames(2)));
}

#endif // WAVEDASHMACRO_H

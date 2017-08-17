#ifndef WAVEDASHMACRO_H
#define WAVEDASHMACRO_H

#include "ActivatorMacro.h"

// This class is a wavedash macro. You can trim the delay time to account for
// different characters having different jumpsquats. I was having some timing
// issues in the game so I had to do a trick where you push L and R spaced apart
// by 1 frame. It seems to work perfectly on console, but on dolphin it can do
// wavedashes a frame late rarely.
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

    uint8_t _airDodgeDelayFrames;

    const uint8_t _minimumDelay;
    const uint8_t _maximumDelay;

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
    _jump.addInput(ActivatorMacroUnit(true, 4));
    _jump.addInput(ActivatorMacroUnit(false, 4));

    // This check is needed for dolphin mode. Characters with Falco's jumpsquat timing
    // or greater need the triggers delayed by one half frame on dolphin. If they aren't
    // delayed it can eat wavedashes. The delay causes bad timing on console so we turn
    // it off there.
    if (_airDodgeDelayFrames < 5 || !HalfFramesElapsed::dolphinMode())
    {
        // L
        _LMacro.setStartDelay(_airDodgeDelayFrames * 2);
        _LMacro.addInput(ActivatorMacroUnit(true, 4));
        _LMacro.addInput(ActivatorMacroUnit(false, 4));

        // R
        _RMacro.setStartDelay(4);
        _RMacro.addInput(ActivatorMacroUnit(false, (_airDodgeDelayFrames - 1) * 2));
        _RMacro.addInput(ActivatorMacroUnit(true, 4));
    }
    else
    {
        // L
        _LMacro.setStartDelay(_airDodgeDelayFrames * 2 + 1);
        _LMacro.addInput(ActivatorMacroUnit(true, 4));
        _LMacro.addInput(ActivatorMacroUnit(false, 4));

        // R
        _RMacro.setStartDelay(4);
        _RMacro.addInput(ActivatorMacroUnit(false, (_airDodgeDelayFrames - 1) * 2 + 1));
        _RMacro.addInput(ActivatorMacroUnit(true, 4));
    }
}

#endif // WAVEDASHMACRO_H
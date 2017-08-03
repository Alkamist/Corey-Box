#ifndef ACTIVATORMACRO_H
#define ACTIVATORMACRO_H

#include "Activator.h"
#include "Timer.h"

// This is the basic unit for an ActivatorMacro. You can pass in anonymous
// instantiations of these to an ActivatorMacro.
struct ActivatorMacroUnit
{
    ActivatorMacroUnit()
    : state(false),
      duration(0)
    {}

    explicit ActivatorMacroUnit(const bool inputState, const uint32_t inputDuration)
    : state(inputState),
      duration(inputDuration)
    {}

    bool state;
    uint32_t duration;
};

// This class is a macro that is specifically meant to run an Activator.
// Use it to build Macros of true and false data. It is currently limited
// to a maximum of 16 inputs.
class ActivatorMacro : public Activator
{
public:
    ActivatorMacro()
    : Activator(),
      _inputIndex(0),
      _readIndex(0),
      _macroLength(0),
      _isInterruptible(false),
      _shouldLoop(false),
      _start(false, 0)
    {
        clearMacro();
    }

    void process();
    void endCycle();

    void setStartDelay(const uint32_t delay)            { _start.duration = delay; }

    void addInput(ActivatorMacroUnit input);
    void clearMacro();

    void setInterruptible(const bool state)             { _isInterruptible = state; }
    void setLooping(const bool state)                   { _shouldLoop = state; }

    const uint8_t getLength() const                     { return _maxLength; }

    const bool isRunning() const                        { return _isRunning; }
    const bool isStarting() const                       { return _isStarting; }
    const bool isInterruptible() const                  { return _isInterruptible; }

    virtual ActivatorMacro& operator=(const bool value) { _activator = value; return *this; }

private:
    Activator _activator;

    uint8_t _inputIndex;
    uint8_t _readIndex;
    uint8_t _macroLength;

    bool _isRunning;
    bool _isStarting;
    bool _isInterruptible;
    bool _shouldLoop;

    const uint8_t _maxLength = 16;

    ActivatorMacroUnit _inputList[16];
    ActivatorMacroUnit _start;

    Timer _timer;

    void runMacro();
    void runStart();
    void startMacro();
};



void ActivatorMacro::process()
{
    bool macroCanStart = (!_isRunning && !_isStarting) || _isInterruptible;

    if (_activator.justActivated() && macroCanStart)
        startMacro();

    if (_isStarting)
        runStart();

    if (_isRunning)
        runMacro();
}

void ActivatorMacro::endCycle()
{
    Activator::endCycle();
    _activator.endCycle();
}

void ActivatorMacro::addInput(ActivatorMacroUnit input)
{
    _inputList[_inputIndex] = input;

    if (_inputIndex < _maxLength - 1)
    {
        ++_macroLength;
        ++_inputIndex;
    }
}

void ActivatorMacro::clearMacro()
{
    for (int index = 0; index < _maxLength; ++index)
    {
        ActivatorMacroUnit defaultUnit;
        _inputList[index] = defaultUnit;
    }

    _start.duration = 0;
    _isStarting = false;
    _isRunning = false;
    _macroLength = 0;
    _readIndex = 0;
    _inputIndex = 0;
}

void ActivatorMacro::startMacro()
{
    _isStarting = true;
    _isRunning = false;
    _readIndex = 0;

    setState(_start.state);
    _timer.setTargetTime(_start.duration);

    _timer.reset();
}

void ActivatorMacro::runStart()
{
    if (_timer)
    {
        _isStarting = false;
        _isRunning = true;
    }
}

void ActivatorMacro::runMacro()
{
    if (_timer)
    {
        setState(_inputList[_readIndex].state);
        _timer.setTargetTime(_inputList[_readIndex].duration);

        _timer.reset();
        ++_readIndex;

        if (_readIndex > _macroLength)
        {
            _readIndex = 0;

            if (!_shouldLoop || !_activator)
                _isRunning = false;
        }
    }
}

#endif // ACTIVATORMACRO_H

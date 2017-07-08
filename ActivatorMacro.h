#ifndef CONTROLMACRO_H
#define CONTROLMACRO_H

#include "Activator.h"
#include "Timer.h"
#include "Array.h"

// This is the basic unit for an ActivatorMacro. You can pass in anonymous
// instantiations of these to an ActivatorMacro.
struct ActivatorMacroUnit
{
    ActivatorMacroUnit()
    : state(false),
      duration(0)
    {}

    explicit ActivatorMacroUnit(const bool inputState, const uint16_t inputDuration)
    : state(inputState),
      duration(inputDuration)
    {}

    bool state;
    uint16_t duration;
};

// This class is a macro that is specifically meant to run an Activator.
// Use it to build Macros of true and false data.
class ActivatorMacro : public Activator
{
public:
    ActivatorMacro()
    : Activator(),
      _isInterruptible(false),
      _shouldLoop(false),
      _start(false, 0)
    {
        clearMacro();
    }

    void process();
    void endCycle();

    void setStartDelay(const uint16_t delay)            { _start.duration = delay; }

    void addInput(ActivatorMacroUnit input)             { _inputList.insertAtEnd(input); }
    void clearMacro();

    void setInterruptible(const bool state)             { _isInterruptible = state; }
    void setLooping(const bool state)                   { _shouldLoop = state; }

    const unsigned int getLength() const                { return _inputList.getLength(); }

    const bool isRunning() const                        { return _isRunning; }
    const bool isStarting() const                       { return _isStarting; }
    const bool isInterruptible() const                  { return _isInterruptible; }

    virtual ActivatorMacro& operator=(const bool value) { _activator = value; return *this; }

private:
    Activator _activator;

    int _inputIndex;

    bool _isRunning;
    bool _isStarting;
    bool _isInterruptible;
    bool _shouldLoop;

    Array<ActivatorMacroUnit> _inputList;

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

void ActivatorMacro::clearMacro()
{
    _inputList.erase();
    _start.duration = 0;
    _isStarting = false;
    _isRunning = false;
    _inputIndex = -1;
}

void ActivatorMacro::startMacro()
{
    _isStarting = true;
    _isRunning = false;
    _inputIndex = -1;
    _timer.reset();

    setState(_start.state);
    _timer.setTargetTime(_start.duration);
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
        _timer.reset();
        ++_inputIndex;

        if (_inputIndex >= _inputList.getLength())
        {
            if (_shouldLoop && _activator)
                _inputIndex = 0;
            else
            {
                _isRunning = false;
                _inputIndex = -1;
                return;
            }
        }

        setState(_inputList[_inputIndex].state);
        _timer.setTargetTime(_inputList[_inputIndex].duration);
    }
}

#endif // CONTROLMACRO_H

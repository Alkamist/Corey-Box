#ifndef CONTROLMACRO_H
#define CONTROLMACRO_H

#include "Activator.h"
#include "Timer.h"
#include "Array.h"

struct ActivatorMacroUnit
{
    explicit ActivatorMacroUnit()
    : state(false),
      duration(0)
    {}

    explicit ActivatorMacroUnit(const bool inputState, const uint64_t inputDuration)
    : state(inputState),
      duration(inputDuration)
    {}

    bool state;
    uint64_t duration;
};

class ActivatorMacro : public Activator
{
public:
    ActivatorMacro(const Activator& activator)
    : Activator(),
      _activator(activator),
      _isInterruptible(false),
      _start(false, 0)
    {
        clearMacro();
    }

    void process();

    void setStartDelay(const uint64_t delay) { _start.duration = delay; }

    void addInput(ActivatorMacroUnit input)  { _inputList.insertAtEnd(input); }
    void clearMacro();

    void setInterruptible(const bool state)  { _isInterruptible = state; }

    const unsigned int getLength() const     { return _inputList.getLength(); }

    const bool isRunning() const             { return _isRunning; }
    const bool isStarting() const            { return _isStarting; }
    const bool isInterruptible() const       { return _isInterruptible; }

private:
    const Activator& _activator;

    int _inputIndex;

    bool _isRunning;
    bool _isStarting;
    bool _isInterruptible;

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
    if (_timer.targetTimeReached())
    {
        _isStarting = false;
        _isRunning = true;
    }
}

void ActivatorMacro::runMacro()
{
    if (_timer.targetTimeReached())
    {
        //_timer.subtractTargetTime();
        _timer.reset();
        ++_inputIndex;

        if (_inputIndex >= _inputList.getLength())
        {
            _isRunning = false;
            _inputIndex = -1;
            return;
        }

        setState(_inputList[_inputIndex].state);
        _timer.setTargetTime(_inputList[_inputIndex].duration);
    }
}

#endif // CONTROLMACRO_H

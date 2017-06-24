#ifndef CONTROLMACRO_H
#define CONTROLMACRO_H

#include "Control.h"
#include "Timer.h"
#include "Array.h"

struct ControlMacroUnit
{
    explicit ControlMacroUnit()
    : control(),
      duration(0)
    {}

    explicit ControlMacroUnit(const bool inputControl, const uint64_t inputDuration)
    : control(inputControl),
      duration(inputDuration)
    {}

    explicit ControlMacroUnit(const Control& inputControl, const uint64_t inputDuration)
    : control(inputControl),
      duration(inputDuration)
    {}

    Control control;
    uint64_t duration;
};

class ControlMacro : public Control
{
public:
    ControlMacro()
    : Control(),
      _isInterruptible(false),
      _start(false, 0)
    {
        clearMacro();
    }

    void update();

    void setControls(const bool activator);
    void setStartDelay(const uint64_t delay) { _start.duration = delay; }

    void addInput(ControlMacroUnit input)    { _inputList.insertAtEnd(input); }
    void clearMacro();

    void setInterruptible(const bool state)  { _isInterruptible = state; }

    const unsigned int getLength() const     { return _inputList.getLength(); }

    const bool isRunning() const             { return _isRunning; }
    const bool isStarting() const            { return _isStarting; }
    const bool isInterruptible() const       { return _isInterruptible; }

private:
    Control _activator;

    int _inputIndex;

    bool _isRunning;
    bool _isStarting;
    bool _isInterruptible;

    Array<ControlMacroUnit> _inputList;

    ControlMacroUnit _start;

    Timer _timer;

    void runMacro();
    void runStart();
    void startMacro();
};



void ControlMacro::update()
{
    Control::update();
    _activator.update();
}

void ControlMacro::setControls(const bool activator)
{
    _activator = activator;

    bool macroCanStart = !_isRunning || _isInterruptible;

    if (_activator.justActivated() && macroCanStart)
        startMacro();

    if (_isStarting)
        runStart();

    if (_isRunning)
        runMacro();
}

void ControlMacro::clearMacro()
{
    _inputList.erase();
    _isStarting = false;
    _isRunning = false;
    _inputIndex = -1;
}

void ControlMacro::startMacro()
{
    _isStarting = true;
    _isRunning = false;
    _inputIndex = -1;
    _timer.reset();

    setValue(_start.control);
    _timer.setTargetTime(_start.duration);
}

void ControlMacro::runStart()
{
    if (_timer.targetTimeReached())
    {
        _isStarting = false;
        _isRunning = true;
    }
}

void ControlMacro::runMacro()
{
    if (_timer.targetTimeReached())
    {
        _timer.subtractTargetTime();
        ++_inputIndex;

        if (_inputIndex >= _inputList.getLength())
        {
            _isRunning = false;
            return;
        }

        setValue(_inputList[_inputIndex].control);
        _timer.setTargetTime(_inputList[_inputIndex].duration);
    }
}

#endif // CONTROLMACRO_H
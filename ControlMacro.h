#ifndef CONTROLMACRO_H
#define CONTROLMACRO_H

#include "ControlValue.h"
#include "ControlMacroUnit.h"
#include "Array.h"

class ControlMacro : public ControlValue
{
public:
    explicit ControlMacro(const ControlValue& activator);

    virtual void update();

    void addInput(ControlMacroUnit input);
    void clearMacro();

    const unsigned int getLength() const        { return _inputList.getLength(); }

    const bool isFinished() const               { return _isFinished; }

private:
    const ControlValue* _activator;

    unsigned int _inputIndex;

    bool _isFinished;
    bool _applyValue;

    Array<ControlMacroUnit> _inputList;

    Timer _timer;

    void runMacro();
    void reset();
};



void ControlMacro::update()
{
    ControlValue::update();

    if (_activator->justActivated() && _isFinished)
        reset();

    if (!_isFinished)
        runMacro();
}

void ControlMacro::addInput(ControlMacroUnit input)
{
    if (_inputIndex > 0)
        input.setDuration(input.getDuration() + _inputList[_inputIndex - 1].getDuration());

    _inputList.insertAtEnd(input);

    ++_inputIndex;
}

void ControlMacro::clearMacro()
{
    _inputList.erase();
    _inputIndex = 0;
}

void ControlMacro::reset()
{
    _applyValue = true;
    _isFinished = false;
    _inputIndex = 0;
    _timer.reset();
}

void ControlMacro::runMacro()
{
    if(_applyValue)
    {
        setValue(_inputList[_inputIndex].getValue());

        _timer.setTargetTime(_inputList[_inputIndex].getDuration());

        _applyValue = false;
    }

    if (_timer.targetTimeReached())
    {
        if (_inputIndex >= _inputList.getLength() - 1)
        {
            _isFinished = true;
            return;
        }

        ++_inputIndex;

        _applyValue = true;
    }
}

ControlMacro::ControlMacro(const ControlValue& activator)
: ControlValue(),
  _activator(&activator),
  _timer(0),
  _inputIndex(0),
  _isFinished(true),
  _applyValue(false)
{}

#endif // CONTROLMACRO_H
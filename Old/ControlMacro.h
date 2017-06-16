#ifndef CONTROLMACRO_H
#define CONTROLMACRO_H

#include "ControlValue.h"
#include "ControlMacroUnit.h"
#include "Array.h"

class ControlMacro : public ControlValue
{
public:
    explicit ControlMacro(const double value,
                          const double center);

    virtual void update();

    void addInput(const ControlMacroUnit input) { _inputList.insertAtEnd(input); }

    const bool _isFinished() const { return _isFinished; }

private:
    UpdatedValue<unsigned int> _inputIndex;

    Array<ControlMacroUnit> _inputList;

    Timer _timer;

    bool _isFinished;
    bool _justStarted;

    unsigned int _endDelay;

    void runMacro();
    void runEnd();
    void checkIfFinished();
};



void ControlMacro::update()
{
    ControlValue::update();
    _inputIndex.update();

    if (_isFinished)
        runEnd();

    if (!_isFinished)
        runMacro();
}

void ControlMacro::runMacro()
{
    if(_inputIndex.hasChanged()
    || _justStarted)
    {
        _timer.setTargetTime(_inputList[_inputIndex].getDelay());
        _justStarted = false;
    }

    if (_timer.targetTimeReached())
    {
        setValue(_inputList[_inputIndex].getValue());

        checkIfFinished();

        ++_inputIndex;
    }
}

void ControlMacro::runEnd()
{
    if(_inputIndex.hasChanged())
        _timer.setTargetTime(_endDelay);
}

void ControlMacro::checkIfFinished()
{
    if (_inputIndex >= _inputList.getLength() - 1)
    {
        _isFinished = true;
        break;
    }
}

ControlMacro::ControlMacro(const double value,
                           const double center)
: ControlValue(value, center),
  _inputIndex(0),
  _endDelay(0),
  _isFinished(false),
  _justStarted(true)
{}

#endif // CONTROLMACRO_H
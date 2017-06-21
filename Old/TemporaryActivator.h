#ifndef TEMPORARYACTIVATOR_
#define TEMPORARYACTIVATOR_

#include "ControlValue.h"

class TemporaryActivator : public ControlValue
{
public:
    TemporaryActivator();
    explicit TemporaryActivator(const unsigned int time, const ControlValue& activator);

    virtual void update();

    void setTime(const unsigned int time)              { _timer.setTargetTime(time); }
    void setActivator(const ControlValue& activator)   { _activator = &activator; }

private:
    const ControlValue* _activator;

    Timer _timer;
};



void TemporaryActivator::update()
{
    ControlValue::update();

    if (_activator != nullptr)
        if (_activator->justActivated())
            _timer.reset();

    setValue(!_timer.targetTimeReached());
}

TemporaryActivator::TemporaryActivator()
: ControlValue(),
  _timer(0),
  _activator(nullptr)
{}

TemporaryActivator::TemporaryActivator(const unsigned int time, const ControlValue& activator)
: ControlValue(),
  _timer(time),
  _activator(&activator)
{}

#endif // TEMPORARYACTIVATOR_
#ifndef ACTIVATOR_H
#define ACTIVATOR_H

class Activator
{
public:
    Activator()
    : _currentState(false),
      _previousState(false)
    {}

    explicit Activator(const bool state)
    : _currentState(state),
      _previousState(state)
    {}

    virtual void endCycle()                              { _previousState = _currentState; }

    virtual void setState(const Activator& state)        { _currentState = state.isActive(); }
    virtual void setState(const bool state)              { _currentState = state; }
    virtual const bool isActive() const                  { return _currentState; }
    virtual const bool justActivated() const             { return stateJustChanged() && isActive(); }
    virtual const bool justDeactivated() const           { return stateJustChanged() && !isActive(); }
    virtual const bool stateJustChanged() const          { return _currentState != _previousState; }

    //=================== OPERATORS ===================

    const Activator& operator =(const Activator& value)  { _currentState = value.isActive(); return *this; }
    const Activator& operator =(const bool value)        { _currentState = value; return *this; }
    operator bool() const                                { return isActive(); }
    const bool operator !() const                        { return !isActive(); }

    const bool operator &&(const bool value) const       { return isActive() && value; }
    const bool operator ||(const bool value) const       { return isActive() || value; }
    const bool operator ==(const bool value) const       { return isActive() == value; }
    const bool operator !=(const bool value) const       { return isActive() != value; }

    const bool operator &&(const Activator& value) const { return isActive() && value.isActive(); }
    const bool operator ||(const Activator& value) const { return isActive() || value.isActive(); }
    const bool operator ==(const Activator& value) const { return isActive() == value.isActive(); }
    const bool operator !=(const Activator& value) const { return isActive() != value.isActive(); }

private:
    bool _currentState;
    bool _previousState;
};

#endif // ACTIVATOR_H
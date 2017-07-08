#ifndef ACTIVATOR_H
#define ACTIVATOR_H

// This class is the basic unit of something that can activate something else.
// Be sure to call endCycle() at the end of the clock cycle, along with anything
// else that uses this logic.
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

    virtual void setState(const bool state)               { _currentState = state; }
    virtual void endCycle()                               { _previousState = _currentState; }

    virtual const bool justActivated() const              { return justChanged() && _currentState; }
    virtual const bool justDeactivated() const            { return justChanged() && !_currentState; }
    virtual const bool justChanged() const                { return _currentState != _previousState; }

    //=================== OPERATORS ===================

    operator bool() const                                 { return _currentState; }

    virtual Activator& operator =(const Activator& value) { _currentState = value._currentState; return *this; }
    virtual Activator& operator =(const bool state)       { _currentState = state; return *this; }

private:
    bool _currentState;
    bool _previousState;
};

#endif // ACTIVATOR_H
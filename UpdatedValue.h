#ifndef UPDATEDVALUE_H
#define UPDATEDVALUE_H

#include "Timer.h"

// This class is a template class for the base value
// type everything uses. It is basically a primitive
// value wrapper that keeps track of its previous
// value last clock cycle, and can tell you whether
// or not it has changed. It also keeps track of how
// long it has been (in ms) since it has changed.
template <class T>
class UpdatedValue
{
public:
    explicit UpdatedValue(const T value)
    : _value(value),
      _previousValue(value),
      _timeSinceChange(0)
    {}

    virtual void update()
    {
        if (hasChanged())
            _timeSinceChange.reset();

        _previousValue = _value;
    }

    virtual void setValue(const T value)       { _value = value; }
    virtual const T getValue() const           { return _value; }
    const T getPreviousValue() const           { return _previousValue; }

    const bool hasChanged() const              { return _value != _previousValue; }
    const unsigned int timeSinceChange() const { return _timeSinceChange.getValue(); }

private:
    T _value;
    T _previousValue;

    Timer _timeSinceChange;
};

#endif // UPDATEDVALUE_H
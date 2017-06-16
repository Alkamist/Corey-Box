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
    explicit UpdatedValue(const T value);

    UpdatedValue<T>& operator=(const T value);
    operator T() const                         { return getValue(); }

    virtual void update();

    const bool hasChanged() const              { return _value != _previousValue; }
    const unsigned int timeSinceChange() const { return _timeSinceChange.getValue(); }

    virtual void setValue(const T value)       { _value = value; }
    const T getValue() const                   { return _value; }
    const T getPreviousValue() const           { return _previousValue; }

private:
    T _value;
    T _previousValue;

    Timer _timeSinceChange;
};


template <typename T>
void UpdatedValue<T>::update()
{
    if (hasChanged())
        _timeSinceChange.reset();

    _previousValue = _value;
}

template <typename T>
UpdatedValue<T>::UpdatedValue(const T value)
: _value(value),
  _previousValue(value),
  _timeSinceChange(0)
{}

template <typename T>
UpdatedValue<T>& UpdatedValue<T>::operator=(const T value)
{
    setValue(value);

    return *this;
}

#endif // UPDATEDVALUE_H
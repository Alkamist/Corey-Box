#ifndef UPDATEDVALUE_H
#define UPDATEDVALUE_H

#include "Timer.h"

// This class is a template class for the base value
// type everything uses. It is basically a primitive
// value wrapper that keeps track of its previous
// value last clock cycle, and can tell you whether
// or not it has changed.
template <class T>
class UpdatedValue
{
public:
    explicit UpdatedValue(const T value)
    : _value(value)
    {}

    UpdatedValue<T>& operator=(const T value)
    {
        setValue(value);

        return *this;
    }

    operator T() const               { return getValue(); }

    virtual void update()            { _previousValue = _value; }

    const bool hasChanged() const    { return _value != _previousValue; }

    void setValue(const T value)     { _value = value; }
    const T getValue() const         { return _value; }
    const T getPreviousValue() const { return _previousValue; }

private:
    T _value;
    T _previousValue;
};

#endif // UPDATEDVALUE_H
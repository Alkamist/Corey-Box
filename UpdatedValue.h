#ifndef UPDATEDVALUE_H
#define UPDATEDVALUE_H

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
    : _value(value),
      _previousValue(value)
    {}

    virtual void update()                { _previousValue = _value; }

    virtual void setValue(const T value) { _value = value; }
    virtual const T getValue() const     { return _value; }
    const T getPreviousValue() const     { return _previousValue; }

    const bool hasChanged() const        { return _value != _previousValue; }

private:
    T _value;
    T _previousValue;
};

#endif // UPDATEDVALUE_H
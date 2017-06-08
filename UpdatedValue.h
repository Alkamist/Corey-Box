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
    UpdatedValue(const T& value)
    : _value(value)
    {}

    UpdatedValue<T>& operator=(const T& value)
    {
        setValue(value);

        return *this;
    }

    operator T()           { return getValue(); }

    void update()          { _previousValue = _value; }

    bool hasChanged()      { return _value != _previousValue; }

    void setValue(T value) { _value = value; }
    T getValue()           { return _value; }
    T getPreviousValue()   { return _previousValue; }

private:
    T _value;
    T _previousValue;
};



/*template <typename T>
void UpdatedValue<T>::update()
{
    _previousValue = _value;
}*/

#endif // UPDATEDVALUE_H
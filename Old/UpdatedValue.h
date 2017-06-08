#ifndef UPDATEDVALUE_H
#define UPDATEDVALUE_H

template <class T>
class UpdatedValue
{
public:
    UpdatedValue();

    void update();
    bool hasChanged()      { return _hasChanged; }

    void setValue(T value) { _value = value; }

    T getValue()           { return _value; }

private:
    T _value;
    T _previousValue;

    bool _hasChanged;
};


template <typename T>
void UpdatedValue<T>::update()
{
    if (_value != _previousValue)
        _hasChanged = true;
    else
        _hasChanged = false;

    _previousValue = _value;
}

#endif // UPDATEDVALUE_H
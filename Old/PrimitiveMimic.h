#ifndef PRIMITIVEMIMIC_H
#define PRIMITIVEMIMIC_H

template <class T>
class PrimitiveMimic
{
public:
    explicit PrimitiveMimic(const T value)
    : _value(value)
    {}

    virtual const T setValue(const T value)             { return _value = value; }
    virtual const T getValue() const                    { return _value; }

    virtual operator T() const                          { return _value; }

    virtual const T operator +() const                  { return _value; }
    virtual const T operator -() const                  { return -_value; }

    virtual const T operator =(const T value)           { return _value = value; }
    virtual const T operator +=(const T value)          { return _value = _value + value; }
    virtual const T operator -=(const T value)          { return _value = _value - value; }
    virtual const T operator *=(const T value)          { return _value = _value * value; }
    virtual const T operator /=(const T value)          { return _value = _value / value; }
    //virtual const T operator %=(const T value)        { return _value = _value % value); }

    virtual const T operator +(const T value) const     { return _value + value; }
    virtual const T operator -(const T value) const     { return _value - value; }
    virtual const T operator *(const T value) const     { return _value * value; }
    virtual const T operator /(const T value) const     { return _value / value; }
    //virtual const T operator %(const T value) const   { return _value % value; }

    virtual const T operator ++()                       { return ++_value; }
    virtual const T operator --()                       { return --_value; }
    virtual const T operator ++(int)                    { return _value++; }
    virtual const T operator --(int)                    { return _value--; }

    virtual const bool operator !() const               { return !_value; }
    virtual const bool operator &&(const T value) const { return _value && value; }
    virtual const bool operator ||(const T value) const { return _value || value; }
    virtual const bool operator ==(const T value) const { return _value == value; }
    virtual const bool operator !=(const T value) const { return _value != value; }
    virtual const bool operator >(const T value) const  { return _value > value; }
    virtual const bool operator <(const T value) const  { return _value < value; }
    virtual const bool operator >=(const T value) const { return _value >= value; }
    virtual const bool operator <=(const T value) const { return _value <= value; }

private:
    T _value;
};

#endif // PRIMITIVEMIMIC_H
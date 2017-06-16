#ifndef ARRAY_H
#define ARRAY_H

template <class T>
class Array
{
public:
    Array();
    explicit Array(unsigned int length);
    ~Array();

    T& operator[](unsigned int index);

    void erase();
    void reallocate(unsigned int newLength);
    void resize(unsigned int newLength);
    void insertBefore(T value, unsigned int index);
    void remove(unsigned int index);
    void insertAtBeginning(T value) { insertBefore(value, 0); }
    void insertAtEnd(T value)       { insertBefore(value, _length); }
    const unsigned int getLength()           { return _length; }

private:
    unsigned int _length;
    T* _data;

    void enforceBounds(unsigned int& index);
};


template <typename T>
Array<T>::Array()
: _length(0),
  _data(nullptr)
{}

template <typename T>
Array<T>::Array(unsigned int length)
: _length(length)
{
    _data = new T[length];
}

template <typename T>
Array<T>::~Array()
{
    delete[] _data;
}

template <typename T>
void Array<T>::erase()
{
    delete[] _data;
     _data = 0;
     _length = 0;
}

template <typename T>
T& Array<T>::operator[](unsigned int index)
{
    enforceBounds(index);

    return _data[index];
}

// This resizes the array, destroying any existing content.
template <typename T>
void Array<T>::reallocate(unsigned int newLength)
{
    erase();

    if (newLength <= 0)
        return;

    _data = new T[newLength];
    _length = newLength;
}

// This resizes the array, keeping existing content intact.
template <typename T>
void Array<T>::resize(unsigned int newLength)
{
    if (newLength == _length)
        return;

    if (newLength <= 0)
    {
        erase();
        return;
    }

    // Now we can assume newLength is at least 1 element.  This algorithm
    // works as follows: First we are going to allocate a new array.  Then we
    // are going to copy elements from the existing array to the new array.
    // Once that is done, we can destroy the old array, and make _data
    // point to the new array.
    T *data = new T[newLength];

    if (_length > 0)
    {
        unsigned int elementsToCopy = (newLength > _length) ? _length : newLength;

        for (unsigned int index = 0; index < elementsToCopy; ++index)
            data[index] = _data[index];
    }

    delete[] _data;

    _data = data;
    _length = newLength;
}

template <typename T>
void Array<T>::insertBefore(T value, unsigned int index)
{
    enforceBounds(index);

    // First create a new array one element larger than the old array.
    T *data = new T[_length + 1];

    // Copy all of the elements up to the index.
    for (unsigned int before = 0; before < index; ++before)
        data[before] = _data[before];

    // Insert our new element into the new array.
    data[index] = value;

    // Copy all of the values after the inserted element.
    for (unsigned int after = index; after < _length; ++after)
        data[after + 1] = _data[after];

    // Finally, delete the old array, and use the new array instead.
    delete[] _data;
    _data = data;
    ++_length;
}

template <typename T>
void Array<T>::remove(unsigned int index)
{
    enforceBounds(index);

    if (_length == 1)
    {
        erase();
        return;
    }

    // First create a new array one element smaller than the old array.
    T *data = new T[_length - 1];

    // Copy all of the elements up to the index.
    for (unsigned int before = 0; before  < index; ++before)
        data[before] = _data[before];

    // Copy all of the values after the removed element.
    for (unsigned int after = index + 1; after < _length; ++after )
        data[after - 1] = _data[after];

    // Finally, delete the old array, and use the new array instead.
    delete[] _data;
    _data = data;
    --_length;
}

template <typename T>
void Array<T>::enforceBounds(unsigned int& index)
{
    if (index < 0)
        index = 0;
    if (index > _length)
        index = _length;
}

#endif // ARRAY_H
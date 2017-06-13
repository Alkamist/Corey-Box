#ifndef ARRAY_H
#define ARRAY_H

template <class T>
class Array
{
public:
    Array();
    explicit Array(int length);
    ~Array();

    T& operator[](int index);

    void erase();
    void reallocate(int newLength);
    void resize(int newLength);
    void insertBefore(T value, int index);
    void remove(int index);
    void insertAtBeginning(T value) { insertBefore(value, 0); }
    void insertAtEnd(T value)       { insertBefore(value, _length); }
    const int getLength()           { return _length; }

private:
    int _length;
    T* _data;

    void enforceBounds(int& index);
};


template <typename T>
Array<T>::Array()
: _length(0),
  _data(nullptr)
{}

template <typename T>
Array<T>::Array(int length)
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
T& Array<T>::operator[](int index)
{
    enforceBounds(index);

    return _data[index];
}

// This resizes the array, destroying any existing content.
template <typename T>
void Array<T>::reallocate(int newLength)
{
    erase();

    if (newLength <= 0)
        return;

    _data = new T[newLength];
    _length = newLength;
}

// This resizes the array, keeping existing content intact.
template <typename T>
void Array<T>::resize(int newLength)
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
        int elementsToCopy = (newLength > _length) ? _length : newLength;

        for (int index = 0; index < elementsToCopy; ++index)
            data[index] = _data[index];
    }

    delete[] _data;

    _data = data;
    _length = newLength;
}

template <typename T>
void Array<T>::insertBefore(T value, int index)
{
    enforceBounds(index);

    // First create a new array one element larger than the old array.
    T *data = new T[_length + 1];

    // Copy all of the elements up to the index.
    for (int before = 0; before < index; ++before)
        data[before] = _data[before];

    // Insert our new element To the new array.
    data[index] = value;

    // Copy all of the values after the inserted element.
    for (int after = index; after < _length; ++after)
        data[after + 1] = _data[after];

    // Finally, delete the old array, and use the new array instead.
    delete[] _data;
    _data = data;
    ++_length;
}

template <typename T>
void Array<T>::remove(int index)
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
    for (int before = 0; before  < index; ++before)
        data[before] = _data[before];

    // Copy all of the values after the removed element.
    for (int after = index + 1; after < _length; ++after )
        data[after - 1] = _data[after];

    // Finally, delete the old array, and use the new array instead.
    delete[] _data;
    _data = data;
    --_length;
}

template <typename T>
void Array<T>::enforceBounds(int& index)
{
    if (index < 0)
        index = 0;
    if (index > _length)
        index = _length;
}

#endif // ARRAY_H
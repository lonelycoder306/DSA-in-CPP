#pragma once
#include <cstddef>
#include <stdexcept>

#define TEMP template<typename T>

TEMP
class Array
{
    private:
        T* entries;
        size_t _count;
        size_t _capacity;

        void shift(int shift, int start = 0);

    public:
        // Constructing Array objects.
        Array();
        Array(int size);
        Array(const Array<T>& other);
        Array(Array<T>&& other);
        Array& operator=(const Array<T>& other);
        Array& operator=(Array<T>&& other);
        ~Array();

        // Basic operators.
        T& operator[](int index); // index < count.
        bool operator==(const Array<T>& other);

        // Utility.
        void grow();

        void add(T element);
        void insert(T element, int index);
        T& pop();
        size_t count();
        size_t capacity();
        T* front();
        T* back();
        T& slot(int index); // index < capacity.

        class iterator
        {
            private:
                T* ptr;
            
            public:
                iterator(T* ptr);
                iterator(const iterator& other);
                iterator& operator=(const iterator& other);

                T& operator*() const;
                iterator& operator++();
                iterator operator++(int);
                iterator& operator--();
                iterator operator--(int);
                bool operator==(const iterator& other);
                bool operator!=(const iterator& other);
        };

        class const_iterator
        {
            private:
                const T* ptr;
            
            public:
                const_iterator(const T* ptr);
                const_iterator(const const_iterator& other);
                const_iterator& operator=(const const_iterator& other);

                const T& operator*() const;
                const_iterator& operator++();
                const_iterator operator++(int);
                const_iterator& operator--();
                const_iterator operator--(int);
                bool operator==(const const_iterator& other) const;
                bool operator!=(const const_iterator& other) const;
        };

        iterator begin();
        iterator end();
        const_iterator cbegin();
        const_iterator cend();
};

TEMP
Array<T>::Array()
{
    entries = nullptr; // Well-defined to call delete[] on nullptr.
    _count = 0;
    _capacity = 0;
}

TEMP
Array<T>::Array(int size)
{
    if (size < 0)
        throw std::out_of_range("Negative index cannot be used.");
    entries = new T[size];
    _count = (size_t) 0; // No elements used at time of construction.
    _capacity = (size_t) size;
}

TEMP
Array<T>::Array(const Array<T>& other)
{
    this->entries = new T[other._capacity];

    for (int i = 0; i < (int) this->_count; i++)
        this->entries[i] = other.entries[i];

    this->_count = other._count;
    this->_capacity = other._capacity;
}

TEMP
Array<T>::Array(Array<T>&& other)
{
    this->entries = other.entries;
    this->_count = other._count;
    this->_capacity = other._capacity;

    other.entries = nullptr;
    other._count = 0;
    other._capacity = 0;
}

TEMP
Array<T>& Array<T>::operator=(const Array<T>& other)
{
    delete[] this->entries;
    this->entries = new T[other._capacity];

    for (int i = 0; i < (int) this->_count; i++)
        this->entries[i] = other.entries[i];

    this->_count = other._count;
    this->_capacity = other._capacity;
    return *this;
}

TEMP
Array<T>& Array<T>::operator=(Array<T>&& other)
{
    delete[] this->entries;

    this->entries = other.entries;
    this->_count = other._count;
    this->_capacity = other._capacity;

    other.entries = nullptr;
    other._count = 0;
    other._capacity = 0;

    return *this;
}

TEMP
Array<T>::~Array()
{
    _count = 0;
    _capacity = 0;
    delete[] entries;
}

TEMP
T& Array<T>::operator[](int index)
{
    if ((index < 0) || ((size_t) index >= _count))
        throw std::out_of_range("Invalid index.");

    return entries[index];
}

TEMP
bool Array<T>::operator==(const Array<T>& other)
{
    if (this->_count != other._count) return false;

    for (int i = 0; i < (int) this->_count; i++)
    {
        if (this->entries[i] != other.entries[i])
            return false;
    }

    return true;
}

TEMP
void Array<T>::grow()
{
    _capacity = (_capacity == 0 ? 8 : _capacity * 2);
    T* newEntries = new T[_capacity];
    for (int i = 0; i < (int) _count; i++)
        newEntries[i] = std::move(entries[i]);
    delete[] entries;
    this->entries = newEntries;
    // this->count does not change.
    // newEntries goes out of scope here.
}

TEMP
void Array<T>::add(T element)
{
    if (_capacity <= _count)
        grow();
    entries[(int) _count++] = element;
}

// When using this function, the unshifted
// buffer area in the array must be replaced/
// initialized immediately to maintain
// the contiguous storage of elements in the array.
TEMP
void Array<T>::shift(int shift, int start)
{
    if ((shift == 0) || (_count == 0)) // Nothing to do.
        return;

    if (shift < 0)
        return; // Throw error?

    if ((start < 0) || (start >= _count))
        return; // Throw error?

    // Shift might be large, so we
    // repeatedly grow until capacity is enough.
    while (_capacity < _count + shift)
        grow();
    
    // To avoid data corruption, we make a
    // new internal array.
    // Note: we don't use an Array<T> local
    // variable since its destructor (called when
    // it goes out of scope by the end of this function)
    // will deallocate the memory we just "filled up".

    T* newEntries = new T[_capacity];
    for (int i = 0; i < start; i++)
        newEntries[i] = this->entries[i];
    
    for (int i = start; i < _count; i++)
        newEntries[i + shift] = this->entries[i];
    delete[] this->entries;
    this->entries = newEntries;
    // Only increment count when we fill the
    // empty spots.
}

TEMP
void Array<T>::insert(T element, int index)
{
    if ((index < 0) || (index >= _count))
        return; // Throw error?
    
    if (_capacity <= _count)
        grow();

    shift(1, index);
    entries[index] = element;
    _count++;
}

TEMP
T& Array<T>::pop()
{
    _count--;
    return entries[(int) _count];
}

TEMP
size_t Array<T>::count()
{
    return _count;
}

TEMP
size_t Array<T>::capacity()
{
    return _capacity;
}

TEMP
T* Array<T>::front()
{
    return entries;
}

TEMP
T* Array<T>::back()
{
    return entries + _count;
}

TEMP
T& Array<T>::slot(int index)
{
    if ((index < 0) || ((size_t) index >= _capacity))
        throw std::out_of_range("Invalid index.");
    return entries[index];
}

// Iterator implementation.

#define arrIter Array<T>::iterator

TEMP
arrIter::iterator(T* ptr) :
    ptr(ptr) {}

TEMP
arrIter::iterator(const arrIter& other) :
    ptr(other.ptr) {}

TEMP
typename arrIter& arrIter::operator=(const arrIter& other)
{
    this->ptr = other.ptr;
    return *this;
}

TEMP
T& arrIter::operator*() const
{
    return *ptr;
}

TEMP
typename arrIter& arrIter::operator++()
{
    ++ptr;
    return *this;
}

TEMP
typename arrIter arrIter::operator++(int n)
{
    arrIter temp = *this;
    ptr++;
    return temp;
}

TEMP
typename arrIter& arrIter::operator--()
{
    --ptr;
    return *this;
}

TEMP
typename arrIter arrIter::operator--(int n)
{
    arrIter temp = *this;
    ptr--;
    return temp;
}

TEMP
bool arrIter::operator==(const arrIter& other)
{
    return (this->ptr == other.ptr);
}

TEMP
bool arrIter::operator!=(const arrIter& other)
{
    return (this->ptr != other.ptr);
}

TEMP
typename arrIter Array<T>::begin()
{
    return iterator(front());
}

TEMP
typename arrIter Array<T>::end()
{
    return iterator(back());
}

// Const iterator implementation.

#define constIter Array<T>::const_iterator

TEMP
constIter::const_iterator(const T* ptr) :
    ptr(ptr) {}

TEMP
constIter::const_iterator(const const_iterator& other) :
    ptr(other.ptr) {}

TEMP
typename constIter& constIter::operator=(const const_iterator& other)
{
    this->ptr = other.ptr;
    return *this;
}

TEMP
const T& constIter::operator*() const
{
    return *ptr;
}

TEMP
typename constIter& constIter::operator++()
{
    ++ptr;
    return *this;
}

TEMP
typename constIter constIter::operator++(int n)
{
    constIter temp = *this;
    ptr++;
    return temp;
}

TEMP
typename constIter& constIter::operator--()
{
    --ptr;
    return *this;
}

TEMP
typename constIter constIter::operator--(int n)
{
    constIter temp = *this;
    ptr--;
    return *this;
}

TEMP
bool constIter::operator==(const const_iterator& other) const
{
    return (this->ptr == other.ptr);
}

TEMP
bool constIter::operator!=(const const_iterator& other) const
{
    return (this->ptr != other.ptr);
}

TEMP
typename constIter Array<T>::cbegin()
{
    return const_iterator(front());
}

TEMP
typename constIter Array<T>::cend()
{
    return const_iterator(end());
}
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
        // Making it inline was the only way to prevent
        // odd compiler/linker errors.
        friend inline void setCount(Array<T>& list, size_t count)
        {
            list._count = count;
        }

        void push(T element);
        int position(T element);
        void insert(T element, int index);
        T erase(int index);
        void remove(T element);
        T pop();
        void popn(int n);
        size_t count();
        size_t capacity();
        T* front();
        T* back();
        T& slot(int index); // index < capacity.
        void slotInsert(T element, int index); // index < capacity.

        class iterator
        {
            private:
                T* ptr;
            
            public:
                iterator(T* ptr);
                iterator(const iterator& other);
                iterator& operator=(const iterator& other);

                T& operator*() const;
                T* operator->() const;
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
                const T* operator->() const;
                const_iterator& operator++();
                const_iterator operator++(int);
                const_iterator& operator--();
                const_iterator operator--(int);
                bool operator==(const const_iterator& other) const;
                bool operator!=(const const_iterator& other) const;
        };

        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
};

TEMP
Array<T>::Array() :
    entries(nullptr), _count(0),
    _capacity(0) {}

TEMP
Array<T>::Array(int size)
{
    if (size < 0)
        throw std::out_of_range("Negative size cannot be used.");
    entries = new T[size];
    _count = 0; // No elements used at time of construction.
    _capacity = static_cast<size_t>(size);
}

TEMP
Array<T>::Array(const Array<T>& other)
{
    this->entries = new T[other._capacity];

    for (size_t i = 0; i < other._capacity; i++)
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
    if (this != &other)
    {
        delete[] this->entries;
        this->entries = new T[other._capacity];

        for (size_t i = 0; i < other._capacity; i++)
            this->entries[i] = other.entries[i];

        this->_count = other._count;
        this->_capacity = other._capacity;
    }

    return *this;
}

TEMP
Array<T>& Array<T>::operator=(Array<T>&& other)
{
    if (this != &other)
    {
        delete[] this->entries;

        this->entries = other.entries;
        this->_count = other._count;
        this->_capacity = other._capacity;

        other.entries = nullptr;
        other._count = 0;
        other._capacity = 0;
    }

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

    for (size_t i = 0; i < this->_count; i++)
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
void Array<T>::push(T element)
{
    if (_capacity <= _count)
        grow();
    entries[(int) _count++] = element;
}

TEMP
int Array<T>::position(T element)
{
    for (size_t i = 0; i < _count; i++)
    {
        if (entries[i] == element)
            return (int) i;
    }

    return -1;
}

// When using this function, any unshifted
// buffer area in the array must be replaced/
// initialized immediately to maintain
// the contiguous storage of elements in the array.
TEMP
void Array<T>::shift(int shift, int start)
{
    if ((shift == 0) || (_count == 0)) // Nothing to do.
        return;

    // We allow the shift to be negative
    // for element removal.
    // Just need to be careful when using
    // that internally.

    // Can't be too negative, though.
    if (shift < (-1 * (int) _count))
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
        throw std::out_of_range("Invalid index.");
    
    if (_capacity <= _count)
        grow();

    shift(1, index);
    entries[index] = element;
    _count++;
}

TEMP
T Array<T>::erase(int index)
{
    if ((index < 0) || (index >= _count))
        throw std::out_of_range("Invalid index.");

    T element = entries[index];
    // Shift begins at the index we pass to shift().
    // We want to move every element *after*
    // the parameter index (here) back, so we add 1.
    shift(-1, index + 1);
    _count--;
    return element;
}

TEMP
void Array<T>::remove(T element)
{
    int index = position(element);
    if (index == -1)
        return;

    erase(index);
}

TEMP
T Array<T>::pop()
{
    _count--;
    return entries[(int) _count];
}

TEMP
void Array<T>::popn(int n)
{
    while (n > 0)
    {
        pop();
        n--;
    }
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

TEMP
void Array<T>::slotInsert(T element, int index)
{
    if ((index < 0) || ((size_t) index >= _capacity))
        throw std::out_of_range("Invalid index");
        
    if (_capacity <= _count)
        grow();

    shift(1, index);
    entries[index] = element;
    _count++;
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
T* arrIter::operator->() const
{
    return ptr;
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

#define constArrIter Array<T>::const_iterator

TEMP
constArrIter::const_iterator(const T* ptr) :
    ptr(ptr) {}

TEMP
constArrIter::const_iterator(const const_iterator& other) :
    ptr(other.ptr) {}

TEMP
typename constArrIter& constArrIter::operator=(const const_iterator& other)
{
    this->ptr = other.ptr;
    return *this;
}

TEMP
const T& constArrIter::operator*() const
{
    return *ptr;
}

TEMP
const T* constArrIter::operator->() const
{
    return ptr;
}

TEMP
typename constArrIter& constArrIter::operator++()
{
    ++ptr;
    return *this;
}

TEMP
typename constArrIter constArrIter::operator++(int n)
{
    constArrIter temp = *this;
    ptr++;
    return temp;
}

TEMP
typename constArrIter& constArrIter::operator--()
{
    --ptr;
    return *this;
}

TEMP
typename constArrIter constArrIter::operator--(int n)
{
    constArrIter temp = *this;
    ptr--;
    return *this;
}

TEMP
bool constArrIter::operator==(const const_iterator& other) const
{
    return (this->ptr == other.ptr);
}

TEMP
bool constArrIter::operator!=(const const_iterator& other) const
{
    return (this->ptr != other.ptr);
}

TEMP
typename constArrIter Array<T>::begin() const
{
    return const_iterator(front());
}

TEMP
typename constArrIter Array<T>::end() const
{
    return const_iterator(back());
}

TEMP
typename constArrIter Array<T>::cbegin() const
{
    return const_iterator(front());
}

TEMP
typename constArrIter Array<T>::cend() const
{
    return const_iterator(back());
}
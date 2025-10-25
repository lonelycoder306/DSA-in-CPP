#pragma once
#include <cstddef>
#include <stdexcept>

#define TEMP template<typename T>

TEMP
class Array
{
    private:
        T* entries;
        size_t count;
        size_t capacity;

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
        T& operator[](int index);
        bool operator==(const Array<T>& other);

        // Utility.
        void resize();

        void add(T element);
        T& pop();
        size_t size();
        size_t maxSize();
        T* begin();
        T* end();
};

TEMP
Array<T>::Array()
{
    entries = new T[0]; // Empty list to start with.
    count = 0;
    capacity = 0;
}

TEMP
Array<T>::Array(int size)
{
    if (size < 0)
        throw std::out_of_range("Negative index cannot be used.");
    entries = new T[size];
    count = (size_t)size;
    capacity = (size_t)size;
}

TEMP
Array<T>::Array(const Array<T>& other)
{
    this->entries = new T[other.count];
    for (int i = 0; i < (int) this->count; i++)
        this->entries[i] = other.entries[i];
    this->count = other.count;
    this->capacity = other.capacity;
}

TEMP
Array<T>::Array(Array<T>&& other)
{
    this->entries = other.entries;
    this->count = other.count;
    this->capacity = other.capacity;

    other.entries = nullptr;
    other.count = 0;
    other.capacity = 0;
}

TEMP
Array<T>& Array<T>::operator=(const Array<T>& other)
{
    delete[] this->entries;
    for (int i = 0; i < (int) this->count; i++)
        this->entries[i] = other.entries[i];
    this->count = other.count;
    this->capacity = other.capacity;
    return *this;
}

TEMP
Array<T>& Array<T>::operator=(Array<T>&& other)
{
    delete[] this->entries;

    this->entries = other.entries;
    this->count = other.count;
    this->capacity = other.capacity;

    other.entries = nullptr;
    other.count = 0;
    other.capacity = 0;

    return *this;
}

TEMP
Array<T>::~Array()
{
    count = 0;
    capacity = 0;
    delete[] entries;
}

TEMP
T& Array<T>::operator[](int index)
{
    // Temporary implementation.
    // No error handling.

    return entries[index];
}

TEMP
bool Array<T>::operator==(const Array<T>& other)
{
    if (this->count != other.count) return false;

    for (int i = 0; i < (int) this->count; i++)
    {
        if (this->entries[i] != other.entries[i])
            return false;
    }

    return true;
}

TEMP
void Array<T>::resize()
{
    capacity = (capacity == 0 ? 8 : capacity * 2);
    T* newEntries = new T[capacity];
    for (int i = 0; i < (int) count; i++)
        newEntries[i] = std::move(entries[i]);
    delete[] entries;
    this->entries = newEntries;
    // newEntries goes out of scope here.
}

TEMP
void Array<T>::add(T element)
{
    if (capacity <= count)
        resize();
    entries[(int) count++] = element;
}

TEMP
T& Array<T>::pop()
{
    count--;
    return entries[(int) count];
}

TEMP
size_t Array<T>::size()
{
    return count;
}

TEMP
size_t Array<T>::maxSize()
{
    return capacity;
}

TEMP
T* Array<T>::begin()
{
    return entries;
}

TEMP
T* Array<T>::end()
{
    return entries + count;
}
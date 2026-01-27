#pragma once
#include <cstring>
#include <stdexcept>

#define TEMP template<typename T>

TEMP
class Deque
{
    private:
        T** map;
        T* begin;
        T* end;
        size_t mapBlocks;
        static constexpr size_t blockSize = 512ULL;
        static constexpr size_t maxObjSize = 1024LL;
        static constexpr size_t defaultMapBlocks = 1ULL;

        static_assert(maxObjSize > sizeof(T), "Object type too large.");

        void allocNewBlock(size_t index);
        void resizeBlockMap(size_t newMapSize);

    public:
        Deque();
        Deque(size_t size);
        Deque(const Deque<T>& other);
        Deque(Deque<T>&& other);
        Deque<T>& operator=(const Deque<T>& other);
        Deque<T>& operator=(Deque<T>&& other);
        ~Deque();

        inline T& operator[](size_t index) const;

        inline void pushFront(const T& obj) noexcept;
        inline void pushBack(const T& obj) noexcept;
        inline void popFront() noexcept;
        inline void popBack() noexcept;

        inline size_t size() const noexcept;
        inline size_t capacity() const noexcept;
};

TEMP
Deque<T>::Deque() :
    map(new T*[defaultMapBlocks]), mapBlocks(defaultMapBlocks)
{
    map[0] = new T[blockSize];
    begin = map[0] + (blockSize / 2);
    end = begin;
    std::memset(&map[1], 0, (defaultMapBlocks - 1) * sizeof(T*));
}

TEMP
Deque<T>::Deque(Deque<T>&& other) :
    map(other.map), begin(other.begin), end(other.end),
    mapBlocks(other.mapBlocks)
{
    other.map = other.begin = other.end = nullptr;
    other.mapBlocks = 0LL;
}

TEMP
Deque<T>& Deque<T>::operator=(Deque<T>&& other)
{
    if (this != &other)
    {
        this->map = other.map;
        this->begin = other.begin;
        this->end = other.end;
        this->mapBlocks = other.mapBlocks;
    
        other.map = other.begin = other.end = nullptr;
        other.mapBlocks = 0;
    }

    return *this;
}

TEMP
Deque<T>::~Deque()
{
    for (size_t i = 0; i < mapBlocks; i++)
        delete[] map[i];
    delete[] map;
}

TEMP
void Deque<T>::allocNewBlock(size_t index)
{
    if (index > mapBlocks) return;
    map[index] = new T[blockSize];
}

// All basic implementations.

TEMP
void Deque<T>::resizeBlockMap(size_t newMapSize)
{

}

TEMP
T& Deque<T>::operator[](size_t index) const
{
    if (index >= end - begin)
        throw std::out_of_range("Invalid index.");

    return begin[index];
}

TEMP
void Deque<T>::pushFront(const T& obj) noexcept
{
    begin--;
    *begin = obj;
}

TEMP
void Deque<T>::pushBack(const T& obj) noexcept
{
    *end = obj;
    end++;
}

TEMP
void Deque<T>::popFront() noexcept
{
    begin++;
}

TEMP
void Deque<T>::popBack() noexcept
{
    end--;
}

TEMP
inline size_t Deque<T>::size() const noexcept
{
    return static_cast<size_t>(end - begin);
}

TEMP
inline size_t Deque<T>::capacity() const noexcept
{
    return mapBlocks * blockSize;
}

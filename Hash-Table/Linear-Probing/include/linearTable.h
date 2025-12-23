#pragma once
#include "../../../Dynamic-Array/include/array.h"
#include "../../General/entryStruct.h"
#include "../../General/hashFunctions.h"
#include <cstdint>
#include <iostream> // For debugging.

#define KVTEMP template<typename Key, typename Value>
#define EKV Entry<Key, Value>

KVTEMP
class linearTable
{
    private:
        // Keeping the same load factor for 
        // both implementations.
        static constexpr double loadFactor = 0.8;
        Array<EKV> entries;
        int count;
        int maxIndex;

        linearTable(int size);

        void reorder();
        void resize();
        // Searches for existing key.
        // Returns reference to available bucket
        // if not found.
        EKV& findSlot(Key key, int* pos);
    
    public:
        linearTable();
        linearTable(const linearTable<Key, Value>& other);
        ~linearTable() = default;
        linearTable<Key, Value>& operator=(const linearTable<Key, Value>& other);
        Value& operator[](Key key);

        void add(Key key, Value value);
        Value* get(Key key);
        void set(Key key, Value value);
        void remove(Key key);
        void merge(const linearTable<Key, Value>& other);
        int size();

        // For debugging.
        void printTable();
};

KVTEMP
linearTable<Key, Value>::linearTable() :
    entries(2), count(0), maxIndex(-1) {}

KVTEMP
linearTable<Key, Value>::linearTable(const linearTable<Key, Value>& other) :
    entries(other.entries), count(other.count),
    maxIndex(other.maxIndex) {}

KVTEMP
linearTable<Key, Value>::linearTable(int size) :
    entries(size), count(0), maxIndex(-1) {}

KVTEMP
linearTable<Key, Value>& linearTable<Key, Value>::
operator=(const linearTable<Key, Value>& other)
{
    this->entries = other.entries;
    return *this;
}

KVTEMP
void linearTable<Key, Value>::reorder()
{
    size_t capacity = entries.capacity();
    linearTable<Key, Value> newTable(static_cast<int>(capacity));
    for (size_t i = 0; i < maxIndex + 1; i++)
    {
        EKV entry = entries.slot((int) i);
        if (entry.state != VALID)
            continue;
        newTable.add(entry.key, entry.value);
    }
    this->entries = newTable.entries;
    this->count = newTable.count; // Might have changed since we dropped tombstones.
    this->maxIndex = newTable.maxIndex;
}

KVTEMP
void linearTable<Key, Value>::resize()
{   
    if ((entries.capacity() * loadFactor) < count + 1)
    {
        if (count == 0)
            entries.grow();
        else
        {
            entries.increaseCapacity();
            reorder();
        }
    }
}

KVTEMP
EKV& linearTable<Key, Value>::findSlot(Key key, int* pos)
{
    uint32_t hash = hashKey(key);
    uint32_t bitmask = (uint32_t) (entries.capacity() - 1);
    int index = (int) (hash & bitmask);

    EKV* tombstone = nullptr;

    EKV* entry = &(entries.slot(index));
    if (pos != nullptr)
        *pos = index;
    while (entry->state != EMPTY)
    {
        if (pos != nullptr)
            *pos = index;
        
        if (entry->key == key)
            return *entry;
        
        if (entry->state == TOMBSTONE)
            tombstone = entry;

        index = (index + 1) & bitmask;
        if (pos != nullptr)
            *pos = index;
        entry = &(entries.slot(index));
    }

    // We didn't find an entry with the key.
    // If we found a tombstone, return it (to reuse it).
    // Otherwise, return the first empty slot we found.
    return (tombstone == nullptr ? *entry : *tombstone);
}

KVTEMP
void linearTable<Key, Value>::add(Key key, Value value)
{
    EKV& existEntry = findSlot(key, nullptr);
    if (existEntry.state == VALID) // Key already exists.
    {
        existEntry.value = value;
        return;
    }

    resize(); // Grow size if needed.

    uint32_t hash = hashKey(key);
    int index;
    EKV& newEntry = findSlot(key, &index);

    if (newEntry.state != TOMBSTONE)
        count++;

    newEntry.key = key;
    newEntry.value = value;
    newEntry.hash = hash;
    newEntry.state = VALID;

    maxIndex = (maxIndex > index ? maxIndex : index);
}

KVTEMP
Value* linearTable<Key, Value>::get(Key key)
{
    if (count == 0) return nullptr;
    
    EKV& entry = findSlot(key, nullptr);
    if (entry.state != VALID)
        return nullptr;
    else
        return &(entry.value);
}

KVTEMP
void linearTable<Key, Value>::set(Key key, Value value)
{
    EKV& entry = findSlot(key, nullptr);
    if (entry.state != VALID)
        add(key, value);
    else
        entry.value = value;
}

KVTEMP
void linearTable<Key, Value>::remove(Key key)
{
    EKV& entry = findSlot(key, nullptr);
    if (entry.state == VALID) // Leave it if it's already empty.
        entry.state = TOMBSTONE;
}

KVTEMP
void linearTable<Key, Value>::merge(const linearTable<Key, Value>& other)
{
    size_t capacity = other.entries.capacity();
    for (size_t i = 0; i < capacity; i++)
    {
        EKV entry = other.entries.slot((int) i);
        if (entry.state == VALID)
            add(entry.key, entry.value);
    }
}

KVTEMP
int linearTable<Key, Value>::size()
{
    return count;
}

KVTEMP
void linearTable<Key, Value>::printTable()
{
    size_t cap = entries.capacity();
    for (size_t i = 0; i < cap; i++)
    {
        std::cout << "Slot " << i << ": ";
        EKV& entry = entries.slot(i);
        if (entry.state == EMPTY)
            std::cout << "EMPTY\n";
        else
        {
            std::cout << "(" << entry.key << ", "
                << entry.value << ")";
            if (entry.state == TOMBSTONE)
                std::cout << " (TOMB)\n";
            else
                std::cout << '\n';
        }
    }
}
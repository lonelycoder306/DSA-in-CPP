/*  This hash table uses open addressing with linear probing and tombstones for deletion, as in the linearTable 
*   container class.
*   However, it additionally includes two optimizations:
*   1. Robin Hood hashing, where we shift entries with shorter probe distances from their expected slot back to insert
*      new entries whose current probe is further than that from *its* expected slot. This causes most probe distances
*      to average out, improving average performance and reducing worst-case degradation.
*   2. SOA structure, where we split the states, hashes, keys, and values into separate arrays. Since most of our
*      searching work or traversal is over the states and hashes, it is more cache friendly to store them in their own
*      arrays where they would be more densely packed. Keys and values are then only accessed if necessary, and directly
*      by index.
*/

#pragma once
#include "../Array/array.h"
#include "entryStruct.h"
#include "hashFunctions.h"
#include <algorithm>
#include <cstdint>
#include <iostream> // For debugging.

#define KVTEMP template<typename Key, typename Value>

KVTEMP
class RobinTable
{
    private:
        // Keeping the same load factor for 
        // all implementations.
        static constexpr double loadFactor = 0.8;
        Array<EntryState> states;
        Array<Hash> hashes;
        Array<Key> keys;
        Array<Value> values;
        size_t count;
        size_t maxIndex;

        RobinTable(size_t size);

        void reorder();
        void resize();
        size_t findSlot(const Key& key);
        inline void insertPair(const Key& key, const Value& value,
            Hash hash, int index);
    
    public:
        RobinTable();
        RobinTable(const RobinTable& other);
        ~RobinTable() = default;
        RobinTable& operator=(const RobinTable& other);
        Value& operator[](const Key& key);

        void add(const Key& key, const Value& value);
        Value* get(const Key& key);
        void set(const Key& key, const Value& value);
        void remove(const Key& key);
        void merge(const RobinTable& other);
        size_t size();

        // For debugging.
        void printTable();
};

KVTEMP
RobinTable<Key, Value>::RobinTable() :
    hashes(2), keys(2), values(2), states(2),
    count(0), maxIndex(SIZE_MAX)
{
    states.fillArray(EMPTY, true); // Mark every slot as empty.
}

KVTEMP
RobinTable<Key, Value>::RobinTable(const RobinTable<Key, Value>& other) :
    hashes(other.hashes), keys(other.keys), values(other.values),
    states(other.states), count(other.count), maxIndex(other.maxIndex) {}

KVTEMP
RobinTable<Key, Value>::RobinTable(size_t size) :
    hashes(size), keys(size), values(size),
    states(size), count(0), maxIndex(SIZE_MAX)
{
    states.fillArray(EMPTY, true);
}

KVTEMP
RobinTable<Key, Value>& RobinTable<Key, Value>::
operator=(const RobinTable<Key, Value>& other)
{
    this->hashes = other.hashes;
    this->keys = other.keys;
    this->values = other.values;
    this->states = other.states;
    this->count = other.count;
    this->maxIndex = other.maxIndex;
}

KVTEMP
void RobinTable<Key, Value>::reorder()
{
    size_t capacity = states.capacity();
    RobinTable<Key, Value> newTable(capacity);
    for (size_t i = 0; i < maxIndex + 1; i++)
    {
        EntryState state = states.slot(i);
        if (state != VALID)
            continue;
        newTable.add(keys.slot(i), values.slot(i));
    }

    this->hashes = newTable.hashes;
    this->keys = newTable.keys;
    this->values = newTable.values;
    this->states = newTable.states;
    this->count = newTable.count; // Might have changed since we dropped tombstones.
    this->maxIndex = newTable.maxIndex;
}

KVTEMP
void RobinTable<Key, Value>::resize()
{   
    if ((states.capacity() * loadFactor) < count + 1)
    {
        if (count == 0)
        {
            hashes.grow();
            keys.grow();
            values.grow();
            states.grow();
        }
        else
        {
            hashes.increaseCapacity();
            keys.increaseCapacity();
            values.increaseCapacity();
            states.increaseCapacity();
            reorder();
        }
    }
}

KVTEMP
size_t RobinTable<Key, Value>::findSlot(const Key& key)
{
    Hash hash = hashKey(key);
    size_t bitmask = hashes.capacity() - 1;
    size_t index = hash & bitmask;
    size_t distance = 0; // Our probe distance while searching for this key.

    EntryState state = states.slot(index);
    while (state != EMPTY)
    {
        size_t origSlot = hashes.slot(index) & bitmask; // Expected slot for current entry.
        size_t currentDiff = index - origSlot +
            (index < origSlot ? hashes.capacity() : 0); // Probe distance for current entry.
        if (currentDiff < distance)
            break;
        
        if ((hashes.slot(index) == hash) && (keys.slot(index) == key))
            return index;
        
        index = (index + 1) & bitmask;
        state = states.slot(index);
        distance++;
    }

    return SIZE_MAX;
}

KVTEMP
inline void RobinTable<Key, Value>::insertPair(const Key& key,
    const Value& value, Hash hash, int index)
{
    keys.slot(index) = key;
    values.slot(index) = value;
    hashes.slot(index) = hash;
    if (states.slot(index) != TOMBSTONE)
        count++;
    states.slot(index) = VALID;
    if (maxIndex == SIZE_MAX)
        maxIndex = index;
    else
        maxIndex = (index > maxIndex ? index : maxIndex);
}

KVTEMP
void RobinTable<Key, Value>::add(const Key& key, const Value& value)
{
    int slot = findSlot(key);
    if (slot != SIZE_MAX)
    {
        values.slot(slot) = value;
        return;
    }

    resize();

    Hash hash = hashKey(key);
    size_t bitmask = hashes.capacity() - 1;
    size_t index = hash & bitmask;
    size_t distance = 0;

    while (true)
    {
        if (states.slot(index) != VALID)
        {
            insertPair(key, value, hash, index);
            return;
        }

        size_t origSlot = hashes.slot(index) & bitmask; // Expected slot for current entry.
        size_t currentDiff = index - origSlot +
            (index < origSlot ? hashes.capacity() : 0); // Probe distance for current entry.
        if (currentDiff < distance)
        {
            std::swap(key, keys.slot(index));
            std::swap(value, values.slot(index));
            std::swap(hash, hashes.slot(index));
            distance = currentDiff;
        }

        index = (index + 1) & bitmask;
        distance++;
    }
}

KVTEMP
Value* RobinTable<Key, Value>::get(const Key& key)
{
    if (count == 0) return nullptr;
    
    size_t slot = findSlot(key);
    if (slot == SIZE_MAX)
        return nullptr;
    else
        return &values.slot(slot);
}

KVTEMP
void RobinTable<Key, Value>::set(const Key& key, const Value& value)
{
    size_t slot = findSlot(key);
    if (slot == SIZE_MAX)
        add(key, value);
    else
        values.slot(slot) = value;
}

KVTEMP
void RobinTable<Key, Value>::remove(const Key& key)
{
    size_t slot = findSlot(key);
    if (slot != SIZE_MAX) // Leave it alone if it's empty.
        states.slot(slot) = TOMBSTONE;
}

KVTEMP
void RobinTable<Key, Value>::merge(const RobinTable<Key, Value>& other)
{
    size_t capacity = other.hashes.capacity();
    for (size_t i = 0; i < capacity; i++)
    {
        if (states.slot(i) == VALID)
            add(other.keys.slot(i), other.values.slot(i));
    }
}

KVTEMP
size_t RobinTable<Key, Value>::size()
{
    return count;
}

KVTEMP
void RobinTable<Key, Value>::printTable()
{
    size_t cap = states.capacity();
    for (size_t i = 0; i < cap; i++)
    {
        std::cout << "Slot " << i << ": ";
        EntryState state = states.slot(i);
        if (state == EMPTY)
            std::cout << "EMPTY\n";
        else
        {
            std::cout << "(" << keys.slot(i) << ", "
                << values.slot(i) << ")";
            if (state == TOMBSTONE)
                std::cout << " (TOMB)\n";
            else
                std::cout << '\n';
        }
    }
}

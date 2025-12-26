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
#include "../../../Dynamic-Array/include/array.h"
#include "../../General/entryStruct.h"
#include "../../General/hashFunctions.h"
#include <algorithm>
#include <cstdint>
#include <iostream> // For debugging.

#define KVTEMP template<typename Key, typename Value>

KVTEMP
class robinTable
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
        int maxIndex;

        robinTable(size_t size);

        void reorder();
        void resize();
        int findSlot(Key key);
        inline void insertPair(Key key, Value value, Hash hash, int index);
    
    public:
        robinTable();
        robinTable(const robinTable<Key, Value>& other);
        ~robinTable() = default;
        robinTable<Key, Value>& operator=(const robinTable<Key, Value>& other);
        Value& operator[](Key key);

        void add(Key key, Value value);
        Value* get(Key key);
        void set(Key key, Value value);
        void remove(Key key);
        void merge(const robinTable<Key, Value>& other);
        size_t size();

        // For debugging.
        void printTable();
};

KVTEMP
robinTable<Key, Value>::robinTable() :
    hashes(2), keys(2), values(2), states(2),
    count(0), maxIndex(-1)
{
    states.fillArray(EMPTY, true); // Mark every slot as empty.
}

KVTEMP
robinTable<Key, Value>::robinTable(const robinTable<Key, Value>& other) :
    hashes(other.hashes), keys(other.keys), values(other.values),
    states(other.states), count(other.count), maxIndex(other.maxIndex) {}

KVTEMP
robinTable<Key, Value>::robinTable(size_t size) :
    hashes(size), keys(size), values(size),
    states(size), count(0), maxIndex(-1)
{
    states.fillArray(EMPTY, true);
}

KVTEMP
robinTable<Key, Value>& robinTable<Key, Value>::
operator=(const robinTable<Key, Value>& other)
{
    this->hashes = other.hashes;
    this->keys = other.keys;
    this->values = other.values;
    this->states = other.states;
    this->count = other.count;
    this->maxIndex = other.maxIndex;
}

KVTEMP
void robinTable<Key, Value>::reorder()
{
    size_t capacity = states.capacity();
    robinTable<Key, Value> newTable(capacity);
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
void robinTable<Key, Value>::resize()
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
int robinTable<Key, Value>::findSlot(Key key)
{
    Hash hash = hashKey(key);
    size_t bitmask = hashes.capacity() - 1;
    int index = hash & bitmask;
    int distance = 0; // Our probe distance while searching for this key.

    EntryState state = states.slot(index);
    while (state != EMPTY)
    {
        int origSlot = hashes.slot(index) & bitmask; // Expected slot for current entry.
        int currentDiff = index - origSlot +
            (index < origSlot ? hashes.capacity() : 0); // Probe distance for current entry.
        if (currentDiff < distance)
            break;
        
        if ((hashes.slot(index) == hash) && (keys.slot(index) == key))
            return index;
        
        index = (index + 1) & bitmask;
        state = states.slot(index);
        distance++;
    }

    return -1;
}

KVTEMP
inline void robinTable<Key, Value>::insertPair(Key key, Value value, Hash hash, int index)
{
    keys.slot(index) = key;
    values.slot(index) = value;
    hashes.slot(index) = hash;
    if (states.slot(index) != TOMBSTONE)
        count++;
    states.slot(index) = VALID;
    maxIndex = (index > maxIndex ? index : maxIndex);
}

KVTEMP
void robinTable<Key, Value>::add(Key key, Value value)
{
    int slot = findSlot(key);
    if (slot != -1)
    {
        values.slot(slot) = value;
        return;
    }

    resize();

    Hash hash = hashKey(key);
    size_t bitmask = hashes.capacity() - 1;
    int index = hash & bitmask;
    int distance = 0;

    while (true)
    {
        if (states.slot(index) != VALID)
        {
            insertPair(key, value, hash, index);
            return;
        }

        int origSlot = hashes.slot(index) & bitmask; // Expected slot for current entry.
        int currentDiff = index - origSlot +
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
Value* robinTable<Key, Value>::get(Key key)
{
    if (count == 0) return nullptr;
    
    int slot = findSlot(key);
    if (slot == -1)
        return nullptr;
    else
        return &values.slot(slot);
}

KVTEMP
void robinTable<Key, Value>::set(Key key, Value value)
{
    int slot = findSlot(key);
    if (slot == -1)
        add(key, value);
    else
        values.slot(slot) = value;
}

KVTEMP
void robinTable<Key, Value>::remove(Key key)
{
    int slot = findSlot(key);
    if (slot != -1) // Leave it alone if it's empty.
        states.slot(slot) = TOMBSTONE;
}

KVTEMP
void robinTable<Key, Value>::merge(const robinTable<Key, Value>& other)
{
    size_t capacity = other.hashes.capacity();
    for (size_t i = 0; i < capacity; i++)
    {
        if (states.slot(i) == VALID)
            add(other.keys.slot(i), other.values.slot(i));
    }
}

KVTEMP
size_t robinTable<Key, Value>::size()
{
    return count;
}

KVTEMP
void robinTable<Key, Value>::printTable()
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

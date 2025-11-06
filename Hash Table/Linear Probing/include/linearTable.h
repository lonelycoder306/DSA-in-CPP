#pragma once
#include "../../../(Dynamic) Array/include/array.h"
#include "../../General/include/entryStruct.h"
#include "../../General/include/hashFunctions.h"
#include <cstdint>

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
        int count = 0;
        int maxIndex = -1;

        linearTable(int size);

        void reorder();
        void resize();
        EKV* getEntry(Key key);
    
    public:
        linearTable() = default;
        linearTable(const linearTable<Key, Value>& other);
        ~linearTable() = default;
        linearTable<Key, Value>& operator=(const linearTable<Key, Value>& other);

        void add(Key key, Value value);
        Value* get(Key key);
        void set(Key key, Value value);
        void remove(Key key);
        void merge(const linearTable<Key, Value>& other);
        int size();
};

KVTEMP
linearTable<Key, Value>::linearTable(const linearTable<Key, Value>& other) :
    entries(other.entries) {}

KVTEMP
linearTable<Key, Value>::linearTable(int size) :
    entries(size) {}

KVTEMP
linearTable<Key, Value>& linearTable<Key, Value>::
operator=(const linearTable<Key, Value>& other)
{
    this->entries = other.entries;
}

KVTEMP
void linearTable<Key, Value>::reorder()
{
    size_t capacity = entries.capacity();
    linearTable<Key, Value> newTable(capacity);
    for (size_t i = 0; i < capacity; i++)
    {
        EKV entry = entries.slot((int) i);
        newTable.add(entry.key, entry.value);
    }
    this->entries = newTable.entries;
    // this->count doesn't change.
}

KVTEMP
void linearTable<Key, Value>::resize()
{   
    if ((entries.capacity() * loadFactor) < count + 1)
    {
        if (maxIndex != -1)
            setCount(this->entries, (size_t) (maxIndex + 1));
        entries.grow();
    }
    else
        return;

    // Need to re-index entire array after this.
    // Descend into each list and extract its node
    // elements to construct the new table.
    
    if (count != 0) // There are elements to reorder.
        reorder();
}

KVTEMP
EKV* linearTable<Key, Value>::getEntry(Key key)
{
    if (count == 0) // No entries to search.
        return nullptr;
    
    uint32_t hash = hashKey(key);
    int index = (int) (hash % entries.capacity());

    if (entries.slot(index).isEmpty) // Deleted entry.
        return nullptr;

    if (entries.slot(index).key == key)
        return &(entries.slot(index));

    while (!entries.slot(index).isEmpty)
    {
        if (entries.slot(index).key == key)
            return &(entries.slot(index));
        index = (index + 1) % entries.capacity();
    }

    if (entries.slot(index).isEmpty) // Didn't find it.
        return nullptr;

    return &(entries.slot(index));
}

KVTEMP
void linearTable<Key, Value>::add(Key key, Value value)
{
    EKV* entryPtr = getEntry(key);
    if (entryPtr != nullptr) // Key already exists.
    {
        entryPtr->value = value;
        return;
    }

    resize(); // Grow size if needed.

    uint32_t hash = hashKey(key);
    int index = (int) (hash % entries.capacity());
    maxIndex = (index > maxIndex) ? index : maxIndex;

    EKV entry(key, value, hash);
    count++;

    if (entries.slot(index).isEmpty) // Bucket is available.
    {
        entries.slot(index) = entry;
        return;
    }

    while (!entries.slot(index).isEmpty)
        index = (index + 1) % entries.capacity();
    
    entries.slot(index) = entry;
    // entries.slotInsert(entry, index);
}

KVTEMP
Value* linearTable<Key, Value>::get(Key key)
{
    EKV* entry = getEntry(key);
    if ((entry == nullptr) /* || (entry->isEmpty)*/)
        return nullptr;
    else
        return &(entry->value);
}

KVTEMP
void linearTable<Key, Value>::set(Key key, Value value)
{
    EKV* entry = getEntry(key);
    if ((entry == nullptr) /* || (entry->isEmpty)*/)
        add(key, value);
    else
        entry->value = value;
}

KVTEMP
void linearTable<Key, Value>::remove(Key key)
{
    uint32_t hash = hashKey(key);
    int index = (int) (hash % entries.capacity());

    if (entries.slot(index).key == key)
    {
        entries.slot(index).isEmpty = true;
        count--;
        return;
    }

    while (!entries.slot(index).isEmpty)
    {
        EKV entry = entries.slot(index);
        if (entry.key == key)
        {
            entry.isEmpty = true;
            count--;
            break;
        }
        index = (index + 1) % entries.capacity();
    }

    // By this point:
    // Key was removed OR key was not found.
}

KVTEMP
void linearTable<Key, Value>::merge(const linearTable<Key, Value>& other)
{
    size_t capacity = other.entries.capacity();
    for (size_t i = 0; i < capacity; i++)
    {
        EKV entry = other.entries.slot((int) i);
        add(entry.key, entry.value);
    }
}

KVTEMP
int linearTable<Key, Value>::size()
{
    return count;
}
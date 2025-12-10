#pragma once
#include "../../../(Dynamic) Array/include/array.h"
#include "../../General/include/entryStruct.h"
#include "../../General/include/hashFunctions.h"
#include "../../../Linked List/Singly Linked List/Regular/include/linkedList.h"
#include <cstdint>

#include <iostream> // FOR DEBUGGING.

#define KVTEMP template<typename Key, typename Value>
#define EKV Entry<Key, Value>
#define EKVList LinkedList<EKV>

KVTEMP
class chainTable
{
    private:
        // Keeping the same load factor for 
        // both implementations.
        static constexpr double loadFactor = 0.8;
        Array<EKVList> entries;
        // Number of buckets (slots in the array) we've used up.
        int bucketCount = 0;
        // Number of key-value pairs we have in the table.
        // Does not necessarily match bucketCount since
        // we can have multiple entries in a single bucket.
        int entryCount = 0;
        // Will mark how far into the array we have entries to copy.
        int maxIndex = -1;

        chainTable(int size);

        void reorder();
        void resize();
        // Adds a key with no value.
        EKV& emptyAdd(Key key);
        EKV* getEntry(Key key);
    
    public:
        chainTable() = default;
        chainTable(const chainTable<Key, Value>& other);
        ~chainTable() = default;
        chainTable<Key, Value>& operator=(const chainTable<Key, Value>& other);
        Value& operator[](Key key);

        void add(Key key, Value value);
        Value* get(Key key);
        void set(Key key, Value value);
        void remove(Key key);
        void merge(const chainTable<Key, Value>& other);

        int bucketSize();
        int entrySize();

        struct Pair
        {
            Key first;
            Value second;

            Pair() = default;
            Pair(EKV entry);
        };

        // Iterates across buckets.
        class iterator
        {
            private:
                Pair* ptr;
            
            public:
                iterator(Pair* ptr);
                iterator(const iterator& other);
                iterator& operator=(const iterator& other);

                Pair& operator*() const;
                iterator& operator++();
                iterator operator++(int);
                iterator& operator--();
                iterator operator--(int);
                bool operator==(const iterator& other);
                bool operator!=(const iterator& other);
        };

        // Iterates across elements of a single bucket.
        class local_iterator
        {
            private:
                Pair* ptr;
                int bucketIndex;
            
            public:
                local_iterator(Pair* ptr);
                local_iterator(const local_iterator& other);
                local_iterator& operator=(const local_iterator& other);

                Pair& operator*() const;
                local_iterator& operator++();
                local_iterator operator++(int);
                local_iterator& operator--();
                local_iterator operator--(int);
                bool operator==(const local_iterator& other);
                bool operator!=(const local_iterator& other);
        };

        class const_iterator
        {
            private:
                const Pair* ptr;
            
            public:
                const_iterator(const Pair* ptr);
                const_iterator(const const_iterator& other);
                const_iterator& operator=(const const_iterator& other);

                const Pair& operator*() const;
                const_iterator& operator++();
                const_iterator operator++(int);
                const_iterator& operator--();
                const_iterator operator--(int);
                bool operator==(const const_iterator& other) const;
                bool operator!=(const const_iterator& other) const;
        };

        class const_local_iterator
        {
            private:
                const Pair* ptr;
                const int bucketIndex;

            public:
                const_local_iterator(const Pair* ptr);
                const_local_iterator(const const_local_iterator& other);
                const_local_iterator& operator=(const const_local_iterator& other);

                const Pair& operator*() const;
                const_local_iterator& operator++();
                const_local_iterator operator++(int);
                const_local_iterator& operator--();
                const_local_iterator operator--(int);
                bool operator==(const const_local_iterator& other) const;
                bool operator!=(const const_local_iterator& other) const;
        };

        // Iterator to first entry in first
        // non-empty bucket.
        iterator begin();
        iterator end();
        const_iterator cbegin();
        const_iterator cend();

        // Iterator to first element of specified bucket.
        local_iterator begin(int n);
        local_iterator end(int n);
        const_local_iterator cbegin(int n);
        const_local_iterator cend(int n);

        // For debugging.
        void printTable();
};

KVTEMP
chainTable<Key, Value>::chainTable(const chainTable<Key, Value>& other)
{
    this->entries = Array<EKVList>(static_cast<int>(other.entries.capacity()));
    size_t size = this->entries.capacity();
    for (size_t i = 0; i < size; i++)
        entries.push(copy(other.entries[i]));
    this->bucketCount = other.bucketCount;
    this->entryCount = other.entryCount;
}

KVTEMP
chainTable<Key, Value>::chainTable(int size) :
    entries(size) {}

KVTEMP
chainTable<Key, Value>& chainTable<Key, Value>
::operator=(const chainTable<Key, Value>& other)
{
    this->entries = Array<EKVList>(static_cast<int>(other.entries.capacity()));
    size_t size = this->entries.capacity();
    for (size_t i = 0; i < size; i++)
        entries.push(copy(other.entries[i]));
    this->bucketCount = other.bucketCount;
    this->entryCount = other.entryCount;
}

KVTEMP
EKV& chainTable<Key, Value>::emptyAdd(Key key)
{
    // This method is only called internally,
    // so we can skip checks for the key existing
    // while being careful to use the method properly.
    
    resize(); // Grow if needed.

    uint32_t hash = hashKey(key);
    uint32_t bitmask = (uint32_t)(entries.capacity() - 1);
    int index = static_cast<int>(hash & bitmask);
    maxIndex = (index > maxIndex) ? index : maxIndex;

    EKVList& list = entries.slot(index);
    if (list.front() == nullptr) // Empty list.
        bucketCount++; // We're filling a new "bucket".

    EKV entry = EKV(key, hash);
    
    list.append(entry);
    entryCount++;
    return list.at(list.length() - 1)->object;
}

KVTEMP
Value& chainTable<Key, Value>::operator[](Key key)
{
    EKV* entry = getEntry(key);
    if (entry == nullptr)
        return emptyAdd(key).value;

    return entry->value;
}

KVTEMP
void chainTable<Key, Value>::reorder()
{
    // Easier to just construct a new table.
    size_t capacity = entries.capacity(); // Cover entire array.
    chainTable<Key, Value> newTable(static_cast<int>(capacity));
    for (size_t i = 0; i < capacity; i++)
    {
        EKVList& list = entries.slot(static_cast<int>(i));
        
        // Handles empty lists as well.
        for (auto* ptr = list.front(); ptr != nullptr; ptr = ptr->next)
        {
            EKV entry = ptr->object;
            newTable.add(entry.key, entry.value);
        }
    }

    this->entries = newTable.entries;
    this->bucketCount = newTable.bucketCount;
    this->maxIndex = newTable.maxIndex; // Re-ordering may have led to a new max index.
    // entryCount doesn't change.
}

// Array will automatically resize if needed, but
// we may need to trigger resizing earlier if
// we reach the load factor.
KVTEMP
void chainTable<Key, Value>::resize()
{
    if ((entries.capacity() * loadFactor) < bucketCount + 1)
    {
        if (maxIndex != -1)
            setCount(this->entries, (size_t) (maxIndex + 1));
        entries.grow();
    }
    else
        return;

    // Need to re-index entire array after resizing.
    // Descend into each list and extract its node
    // elements to construct the new table.
    
    if (entryCount != 0) // There are elements to reorder.
        reorder();
}

KVTEMP
EKV* chainTable<Key, Value>::getEntry(Key key)
{
    if (entryCount == 0)
        return nullptr;
    
    uint32_t hash = hashKey(key);
    uint32_t bitmask = (uint32_t)(entries.capacity() - 1);
    int index = static_cast<int>(hash & bitmask);
    EKVList& list = entries.slot(index);
    
    EKV temp(key, hash); // temp.value uninitialized.

    auto* nodePtr = list.get(temp);
    if (nodePtr == nullptr)
        return nullptr;

    return &(nodePtr->object);
}

KVTEMP
void chainTable<Key, Value>::add(Key key, Value value)
{
    EKV* temp = getEntry(key);
    if (temp != nullptr) // Key already exists.
    {
        temp->value = value;
        return;
    }
    
    resize(); // Grow if needed.

    uint32_t hash = hashKey(key);
    uint32_t bitmask = (uint32_t)(entries.capacity() - 1);
    int index = static_cast<int>(hash & bitmask);
    maxIndex = (index > maxIndex) ? index : maxIndex;

    EKVList& list = entries.slot(index);
    if (list.front() == nullptr) // Empty list.
        bucketCount++; // We're filling a new "bucket".

    EKV entry(key, value, hash);
    
    list.append(entry);
    entryCount++;
}

KVTEMP
Value* chainTable<Key, Value>::get(Key key)
{
    EKV* entry = getEntry(key);
    if (entry == nullptr)
        return nullptr;
    else
        return &(entry->value);
}

KVTEMP
void chainTable<Key, Value>::set(Key key, Value value)
{
    EKV* entry = getEntry(key);

    if (entry == nullptr) // Key does not exist.
        add(key, value);
    else
        entry->value = value;
}

KVTEMP
void chainTable<Key, Value>::remove(Key key)
{
    if (entryCount == 0)
        return;
    
    uint32_t hash = hashKey(key);
    uint32_t bitmask = (uint32_t)(entries.capacity() - 1);
    int index = static_cast<int>(hash & bitmask);
    EKVList& list = entries.slot(index);
    EKV temp = EKV(key, hash);
    if (!list.has(temp))
        return;
    
    list.remove(temp);
    entryCount--;
    if (list.front() == nullptr) // List only had one node.
        bucketCount--;
}

KVTEMP
void chainTable<Key, Value>::merge(const chainTable<Key, Value>& other)
{
    size_t capacity = other.entries.capacity();
    for (size_t i = 0; i < capacity; i++)
    {
        EKVList& list = other.entries.slot(static_cast<int>(i));
        if (list.front() == nullptr)
            continue;

        for (auto* ptr = list.front(); ptr != nullptr; ptr = ptr->next)
        {
            EKV entry = ptr->object;
            add(entry.key, entry.value);
        }
    }
}

KVTEMP
int chainTable<Key, Value>::bucketSize()
{
    return bucketCount;
}

KVTEMP
int chainTable<Key, Value>::entrySize()
{
    return entryCount;
}

// Pair struct.
KVTEMP
chainTable<Key, Value>::Pair::Pair(EKV entry) :
    first(entry.key), second(entry.value) {}

// For debugging.
KVTEMP
void chainTable<Key, Value>::printTable()
{
    for (size_t i = 0; i < entries.capacity(); i++)
    {
        EKVList& list = entries.slot(static_cast<int>(i));
        if (list.front() == nullptr)
            std::cout << "Slot " << i << ": EMPTY\n";
        else
        {
            std::cout << "Slot " << i << ": ";
            for (auto& pair : list)
                std::cout << "(" << pair.key << ", " << pair.value << ")->";
            std::cout << '\n';
        }
    }
}
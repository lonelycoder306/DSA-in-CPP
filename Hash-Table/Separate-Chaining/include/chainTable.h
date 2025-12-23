/*  This hash table implementation uses a basic separate chaining approach for hash collisions.
*   Each bucket contains a (non-intrusive/regular) linked list (implemented in this project),
*   which is traversed to search for a key match upon a collision occurring.
*/

#pragma once
#include "../../../Dynamic-Array/include/array.h"
#include "../../General/entryStruct.h"
#include "../../General/hashFunctions.h"
#include "../../../Linked-List/Singly-Linked-List/Regular/include/linkedList.h"
#include <cstdint>
#include <iostream> // FOR DEBUGGING.

#define KVHTEMP             template<typename Key, typename Value, typename HashFunc>
#define KVHTEMP_DEFAULT     template<typename Key, typename Value, typename HashFunc = Hasher<Key>>
#define EKV                 Entry<Key, Value>
#define EKVList             LinkedList<EKV>

template<typename Key>
struct Hasher
{
    Hash operator()(Key key)
    {
        return hashKey(key);
    }

    Hash operator()(Key key, size_t size)
    {
        return hashKey(key, size);
    }
};

KVHTEMP_DEFAULT
class chainTable
{
    private:
        HashFunc getHash;
        // Keeping the same load factor for 
        // both implementations.
        static constexpr double loadFactor = 0.8;
        Array<EKVList> entries;
        // Number of buckets (slots in the array) we've used up.
        int bucketCount;
        // Number of key-value pairs we have in the table.
        // Does not necessarily match bucketCount since
        // we can have multiple entries in a single bucket.
        int entryCount;
        // Will mark how far into the array we have entries to copy.
        int maxIndex;

        chainTable(int size);

        void reorder();
        void resize();
        // Adds a key with no value.
        EKV& emptyAdd(Key key);
        EKV* getEntry(Key key);
    
    public:
        chainTable();
        chainTable(const chainTable<Key, Value, HashFunc>& other);
        ~chainTable() = default;
        chainTable<Key, Value, HashFunc>& operator=(const chainTable<Key, Value, HashFunc>& other);
        Value& operator[](Key key);

        void add(Key key, Value value);
        Value* get(Key key);
        void set(Key key, Value value);
        void remove(Key key);
        void merge(const chainTable<Key, Value, HashFunc>& other);

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
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;

        // Iterators within specified bucket.
        local_iterator begin(int n);
        local_iterator end(int n);
        const_local_iterator begin(int n) const;
        const_local_iterator end(int n) const;
        const_local_iterator cbegin(int n) const;
        const_local_iterator cend(int n) const;

        // For debugging.
        void printTable();
};

KVHTEMP
chainTable<Key, Value, HashFunc>::chainTable() :
    getHash(HashFunc()), bucketCount(0), entryCount(0),
    maxIndex(-1) {}

KVHTEMP
chainTable<Key, Value, HashFunc>::chainTable(const chainTable<Key, Value, HashFunc>& other)
{
    this->entries = Array<EKVList>(static_cast<int>(other.entries.capacity()));
    size_t size = this->entries.capacity();
    for (size_t i = 0; i < size; i++)
        entries.push(copy(other.entries[i]));
    this->bucketCount = other.bucketCount;
    this->entryCount = other.entryCount;
}

KVHTEMP
chainTable<Key, Value, HashFunc>::chainTable(int size) :
    getHash(HashFunc()), entries(size), bucketCount(0),
    entryCount(0), maxIndex(-1) {}

KVHTEMP
chainTable<Key, Value, HashFunc>& chainTable<Key, Value, HashFunc>
::operator=(const chainTable<Key, Value, HashFunc>& other)
{
    this->entries = Array<EKVList>(static_cast<int>(other.entries.capacity()));
    size_t size = this->entries.capacity();
    for (size_t i = 0; i < size; i++)
        entries.push(copy(other.entries[i]));
    this->bucketCount = other.bucketCount;
    this->entryCount = other.entryCount;
}

KVHTEMP
EKV& chainTable<Key, Value, HashFunc>::emptyAdd(Key key)
{
    // This method is only called internally,
    // so we can skip checks for the key existing
    // while being careful to use the method properly.
    
    resize(); // Grow if needed.

    uint32_t hash = getHash(key);
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

KVHTEMP
Value& chainTable<Key, Value, HashFunc>::operator[](Key key)
{
    EKV* entry = getEntry(key);
    if (entry == nullptr)
        return emptyAdd(key).value;

    return entry->value;
}

KVHTEMP
void chainTable<Key, Value, HashFunc>::reorder()
{
    // Easier to just construct a new table.
    size_t capacity = entries.capacity(); // Cover entire array.
    chainTable<Key, Value, HashFunc> newTable(static_cast<int>(capacity));
    for (size_t i = 0; i < this->maxIndex + 1; i++)
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
KVHTEMP
void chainTable<Key, Value, HashFunc>::resize()
{
    if ((entries.capacity() * loadFactor) < bucketCount + 1)
    {
        if (entryCount == 0)
            entries.grow();
        else
        {
            entries.increaseCapacity();
            reorder();
        }
    }
}

KVHTEMP
EKV* chainTable<Key, Value, HashFunc>::getEntry(Key key)
{
    if (entryCount == 0)
        return nullptr;
    
    uint32_t hash = getHash(key);
    uint32_t bitmask = (uint32_t)(entries.capacity() - 1);
    int index = static_cast<int>(hash & bitmask);
    EKVList& list = entries.slot(index);
    
    EKV temp(key, hash); // temp.value uninitialized.

    auto* nodePtr = list.get(temp);
    if (nodePtr == nullptr)
        return nullptr;

    return &(nodePtr->object);
}

KVHTEMP
void chainTable<Key, Value, HashFunc>::add(Key key, Value value)
{
    EKV* temp = getEntry(key);
    if (temp != nullptr) // Key already exists.
    {
        temp->value = value;
        return;
    }
    
    resize(); // Grow if needed.

    uint32_t hash = getHash(key);
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

KVHTEMP
Value* chainTable<Key, Value, HashFunc>::get(Key key)
{
    EKV* entry = getEntry(key);
    if (entry == nullptr)
        return nullptr;
    else
        return &(entry->value);
}

KVHTEMP
void chainTable<Key, Value, HashFunc>::set(Key key, Value value)
{
    EKV* entry = getEntry(key);

    if (entry == nullptr) // Key does not exist.
        add(key, value);
    else
        entry->value = value;
}

KVHTEMP
void chainTable<Key, Value, HashFunc>::remove(Key key)
{
    if (entryCount == 0)
        return;
    
    uint32_t hash = getHash(key);
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

KVHTEMP
void chainTable<Key, Value, HashFunc>::merge(const chainTable<Key, Value, HashFunc>& other)
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

KVHTEMP
int chainTable<Key, Value, HashFunc>::bucketSize()
{
    return bucketCount;
}

KVHTEMP
int chainTable<Key, Value, HashFunc>::entrySize()
{
    return entryCount;
}

// Pair struct.
KVHTEMP
chainTable<Key, Value, HashFunc>::Pair::Pair(EKV entry) :
    first(entry.key), second(entry.value) {}

// For debugging.
KVHTEMP
void chainTable<Key, Value, HashFunc>::printTable()
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
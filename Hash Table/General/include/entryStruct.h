#pragma once
#include <cstdint>

#define KVTEMP template<typename Key, typename Value>

enum EntryState
{
    VALID,
    TOMBSTONE,
    EMPTY
};

KVTEMP
struct Entry
{
    Key key;
    Value value;
    uint32_t hash;
    EntryState state;

    Entry();
    Entry(Key key, Value value);
    Entry(Key key, uint32_t hash);
    Entry(Key key, Value value, uint32_t hash);
    ~Entry() = default;
    bool operator==(const Entry& other) const;
};

KVTEMP
Entry<Key, Value>::Entry() :
    state(EMPTY) {}

KVTEMP
Entry<Key, Value>::Entry(Key key, Value value) :
    key(key), value(value), state(VALID) {}

KVTEMP
Entry<Key, Value>::Entry(Key key, uint32_t hash) :
    key(key), hash(hash), state(VALID) {}

KVTEMP
Entry<Key, Value>::Entry(Key key, Value value, uint32_t hash) :
    key(key), value(value), hash(hash), state(VALID) {}

KVTEMP
bool Entry<Key, Value>::operator==(const Entry& other) const
{
    return ((this->hash == other.hash) && // For short-circuit evaluation.
            (this->key == other.key)); // Cannot add a key twice.
}
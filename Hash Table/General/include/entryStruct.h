#pragma once
#include <cstdint>

#define KVTEMP template<typename Key, typename Value>

KVTEMP
struct Entry
{
    Key key;
    Value value;
    uint32_t hash;
    bool isEmpty;

    Entry();
    Entry(Key key, Value value);
    Entry(Key key, uint32_t hash);
    Entry(Key key, Value value, uint32_t hash);
    ~Entry() = default;
    bool operator==(const Entry<Key, Value>& other);
};

KVTEMP
Entry<Key, Value>::Entry() :
    isEmpty(true) {}

KVTEMP
Entry<Key, Value>::Entry(Key key, Value value) :
    key(key), value(value), isEmpty(false) {}

KVTEMP
Entry<Key, Value>::Entry(Key key, uint32_t hash) :
    key(key), hash(hash), isEmpty(false) {}

KVTEMP
Entry<Key, Value>::Entry(Key key, Value value, uint32_t hash) :
    key(key), value(value), hash(hash), isEmpty(false) {}

KVTEMP
bool Entry<Key, Value>::operator==(const Entry<Key, Value>& other)
{
    return ((this->hash == other.hash) && // For short-circuit evaluation.
            (this->key == other.key)); // Cannot add a key twice.
}
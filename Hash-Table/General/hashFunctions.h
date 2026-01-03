#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>

using Hash = uint32_t;

template<typename Key>
Hash hashKey(Key key, size_t size = -1);
template<typename T>
Hash hashNumeric(T key);
Hash hashChar(char key);
Hash hashString(std::string_view string);
Hash hashCStr(const char* string, size_t length = -1);

// Need to have the implementations here since
// this file is #included in source files
// with template implementations.

template<typename Key>
Hash hashKey(Key key, size_t size)
{
    if constexpr (std::is_arithmetic_v<Key>)
        return hashNumeric(key);
    if constexpr (std::is_same_v<Key, double>)
        return hashDouble(key);
    if constexpr (std::is_same_v<Key, char>)
        return hashChar(key);
    if constexpr (std::is_same_v<Key, std::string_view>)
        return hashString(key);
    if constexpr (std::is_same_v<Key, std::string>)
        return hashString(key);
    if constexpr (std::is_same_v<Key, const char *>)
        return hashCStr(key);
    if constexpr (std::is_same_v<Key, char *>)
        return hashCStr(key, size);
    return 0; // Error return.
}

// Using Jenkins' one-at-a-time function.
static Hash hashBytes(const uint8_t* bytes, size_t size)
{
    Hash hash = 0;

    for (size_t i = 0; i < size; i++)
    {
        hash += bytes[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

template<typename T>
Hash hashNumeric(T key)
{
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&key);
    return hashBytes(bytes, sizeof(T));
}

Hash hashChar(char key)
{
    return (uint32_t) key;
}

Hash hashString(std::string_view string)
{
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(string.data());
    return hashBytes(bytes, string.size());
}

Hash hashCStr(const char* string, size_t length)
{
    if (length == -1)
        length = strlen(string); // Null-terminated.
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(string);
    return hashBytes(bytes, length);
}
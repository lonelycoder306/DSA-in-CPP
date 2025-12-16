#include "../include/chainTable.h"
#include <chrono>
#include <iostream>

static const char* names[] = {
    "Alice",
    "Bob",
    "John",
    "Maddy"
};

// Random hash function for testing.
struct charHash
{
    Hash operator()(const char* str)
    {
        uint32_t hash = 5381;
        int c;

        while ((c = *str++)) {
            // hash * 33 + c is equivalent to (hash << 5) + hash + c
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }
};

int main()
{
    chainTable<const char *, int, charHash> chartable;

    for (int i = 1; i < 5; i++)
    {
        chartable[names[i - 1]] = i;
        std::cout << "Added " << names[i - 1] << ".\n";
    }

    std::cout << "Buckets: " << chartable.bucketSize() << '\n';
    std::cout << "Size: " << chartable.entrySize() << '\n';
    for (int i = 1; i < 5; i++)
        std::cout << chartable[names[i - 1]] << '\n';

    chartable["Mary"] = 12;
    std::cout << chartable["Mary"] << '\n';
    chartable.remove("Mary");
    if (chartable.get("Mary") == nullptr)
        std::cout << "Removed key successfully.\n";

    using namespace std::chrono;

    chainTable<int, int> table;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 500; i++)
        table[i] = i + 1;
    
    for (int i = 0; i < 500; i++)
        std::cout << *table.get(i) << '\n';

    for (int i = 0; i < 500; i++)
        table.remove(i);

    auto finish = high_resolution_clock::now();
    auto time = duration_cast<milliseconds>(finish - start);
    std::cout << "Time: " << (long double) time.count() / 1000 << '\n';

    return 0;
}
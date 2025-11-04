#include "../include/chainTable.h"
#include <iostream>

static const char* names[] = {
    "Alice",
    "Bob",
    "John",
    "Maddy"
};

int main()
{
    chainTable<const char *, int> table;

    for (int i = 1; i < 5; i++)
    {
        table.add(names[i - 1], i);
        std::cout << "Added " << names[i - 1] << ".\n";
    }

    std::cout << "Buckets: " << table.bucketSize() << '\n';
    std::cout << "Size: " << table.entrySize() << '\n';
    for (int i = 1; i < 5; i++)
        std::cout << *table.get(names[i - 1]) << '\n';
    return 0;
}
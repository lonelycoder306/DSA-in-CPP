#include "linearTable.h"
#include <iostream>

int main()
{
    LinearTable<int, int> table;
    for (int i = 0; i < 10; i++)
        table[i] = i;

    std::cout << "Size: " << table.size() << '\n';

    for (auto& [k, v] : table)
        std::cout << k << ':' << v << '\n';
}
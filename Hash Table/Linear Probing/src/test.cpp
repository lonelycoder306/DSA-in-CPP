#include "../include/linearTable.h"
#include <iostream>

int main()
{
    linearTable<char, int> table;
    for (int i = 1; i < 11; i++)
    {
        table.add((char) (i + 64), i * 2);
        std::cout << "Added " << (char) (i + 64) << ".\n";
    }

    std::cout << "Size: " << table.size() << '\n';
    for (int i = 1; i < 11; i++)
        std::cout << table.get((char)(i + 64)) << '\n';

    table.add((char) 75, 24);
    std::cout << (char) 75 << ":" << *table.get((char) 75) << '\n';
    table.remove((char) 75);
    if (table.get((char) 75) == nullptr)
        std::cout << "Removed key successfully.\n";

    return 0;
}
#include "../include/linearTable.h"
#include <iostream>
#include <chrono>

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
        std::cout << *table.get((char)(i + 64)) << '\n';

    table.add((char) 65, 24);
    std::cout << (char) 65 << ":" << *table.get((char) 65) << '\n';
    table.remove((char) 65);
    if (table.get((char) 65) == nullptr)
        std::cout << "Removed key successfully.\n";

    using namespace std::chrono;

    linearTable<int, int> table2;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 500; i++)
        table2.add(i, i + 1);

    for (int i = 0; i < 500; i++)
        std::cout << *table2.get(i) << '\n';

    for (int i = 0; i < 500; i++)
        table2.remove(i);

    auto finish = high_resolution_clock::now();
    auto time = duration_cast<milliseconds>(finish - start);
    std::cout << "Time: " << (long double)time.count() / 1000 << '\n';

    return 0;
}
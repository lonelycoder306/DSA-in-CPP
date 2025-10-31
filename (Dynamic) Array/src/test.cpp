#include "../include/array.h"
#include <iostream>

int main()
{
    Array<int> array0;
    std::cout << "Array 0 Size: " << array0.count() << '\n';
    std::cout << "Array 0 Capacity: " << array0.capacity() << '\n';

    Array<int> array1(0);
    for (int i = 0; i < 10; i++)
        array1.add(i + 1);
    array1.insert(-1, 3);
    for (const int i : array1)
        std::cout << i << '\n';
    array1.pop();
    
    Array<int> array2 = array1;
    std::cout << "Array 1 Size: " << array1.count() << '\n';
    std::cout << "Array 1 Capacity: " << array1.capacity() << '\n';
    std::cout << "Array 2 Size: " << array2.count() << '\n';
    std::cout << "Array 2 Capacity: " << array2.capacity() << '\n';
    return 0;
}
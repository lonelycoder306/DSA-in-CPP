#include "../include/array.h"
#include <iostream>

int main()
{
    Array<int> array0;
    std::cout << "Array 0 Size: " << array0.size() << '\n';
    std::cout << "Array 0 Capacity: " << array0.maxSize() << '\n';

    Array<int> array1(0);
    for (int i = 0; i < 10; i++)
        array1.add(i + 1);
    for (int* i = array1.begin(); i < array1.end(); i++)
        std::cout << *i << '\n';
    array1.pop();
    
    Array<int> array2 = array1;
    std::cout << "Array 1 Size: " << array1.size() << '\n';
    std::cout << "Array 1 Capacity: " << array1.maxSize() << '\n';
    std::cout << "Array 2 Size: " << array2.size() << '\n';
    std::cout << "Array 2 Capacity: " << array2.maxSize() << '\n';
    return 0;
}
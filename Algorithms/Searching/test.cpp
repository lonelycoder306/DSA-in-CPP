#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifndef SIZE
    #define SIZE 10'000'000
#endif

#ifndef SEARCH
    #define search() 0
#endif

#include "binary.h"
#include "linear.h"

int main()
{
    using clock = std::chrono::high_resolution_clock;
    using nano = std::chrono::nanoseconds;
    using std::chrono::duration_cast;
    
    srand(time(0));
    
    int* nums = new int[SIZE];
    for (int i = 0; i < SIZE; i++)
        nums[i] = i + 1;

    int elem = nums[rand() % SIZE];

    auto start = clock::now();
    volatile int index = search(nums, SIZE, elem);
    auto finish = clock::now();

    if (index != -1)
    {
        auto time = duration_cast<nano>(finish - start);
        printf("Time: %.10Lf\n", (long double) time.count() / 1'000'000'000);
        delete[] nums;
        return 0;
    }
    else
    {
        std::cout << "Failed to find element.\n";
        printf("Element: %d\n", elem);
        printf("Index: %d\n", index);
        delete[] nums;
        return 1;
    }
}
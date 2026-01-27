#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifndef SIZE
    #define SIZE 10'000'000
#endif

#include "../Sorting/insertion.h"
#include "binary.h"

template<typename T>
bool isSorted(T* nums, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        if (nums[i] > nums[i + 1])
            return false;
    }

    return true;
}

template<typename T>
void print(T* nums, int count)
{
    for (int i = 0; i < count; i++)
        std::cout << nums[i] << '\n';
}

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
    volatile int index = search<int>(nums, SIZE, elem);
    auto finish = clock::now();

    if (isSorted(nums, SIZE))
    {
        auto time = duration_cast<nano>(finish - start);
        printf("Time: %.10Lf\n", (long double) time.count() / 1'000'000'000);
        delete[] nums;
        return 0;
    }
    else
    {
        std::cout << "Failed to sort array.\n";
        print(nums, SIZE);
        delete[] nums;
        return 1;
    }
}
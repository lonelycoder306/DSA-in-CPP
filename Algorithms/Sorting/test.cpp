#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifndef SIZE
    #define SIZE 1000
#endif

#include "insertion.h"

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
        nums[i] = rand() % 1000 + 1;

    auto start = clock::now();
    sort<int>(nums, SIZE);
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
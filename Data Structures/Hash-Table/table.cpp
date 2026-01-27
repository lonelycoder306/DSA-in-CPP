#include "chainTable.h"
#include "linearTable.h"
#include "robinTable.h"
#include <chrono>
#include <iostream>

#ifndef TABLE
	#define TABLE chainTable
#endif

#ifndef SIZE
	#define SIZE 1000
#endif

int main()
{
	using namespace std::chrono;
	auto start = high_resolution_clock::now();

	TABLE<int, int> table;

	for (int i = 0; i < SIZE; i++)
		table[i] = i+1;

	for (int i = 0; i < SIZE; i++)
		std::cout << *table.get(i) << '\n';

	for (int i = 0; i < SIZE; i++)
		table.remove(i);

	auto finish = high_resolution_clock::now();
	auto time = duration_cast<milliseconds>(finish - start);
	std::cout << "Time: " << (long double) time.count() / 1000 << '\n';
	return 0;
}

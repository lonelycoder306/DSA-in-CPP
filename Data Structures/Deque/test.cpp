#include "deque.h"
#include <iostream>

int main()
{
	Deque<int> d;
	std::cout << "Size: " << d.size() << '\n';
	std::cout << "Cap: " << d.capacity() << '\n';

	for (int i = 0; i < 10; i++)
		d.pushBack(i + 1);
	for (int i = 0; i < 10; i++)
		std::cout << d[i] << '\n';

	std::cout << "Size: " << d.size() << '\n';
    std::cout << "Cap: " << d.capacity() << '\n';

	for (int i = 0; i < 10; i++)
		d.popBack();

	std::cout << "Size: " << d.size() << '\n';
    std::cout << "Cap: " << d.capacity() << '\n';

	return 0;
}

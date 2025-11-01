#include "../include/linkedList.h"
#include <iostream>

int main()
{
    LinkedList<int> list;

    list.insert(1, 2); // Inserting at position 2 in empty list; doesn't do anything.
    list.insert(1, 0); // Inserting at position 0 in empty list.
    std::cout << list.at(0)->object << '\n';
    std::cout << "List has 1: " << (list.has(1) ? "true" : "false") << '\n';
    std::cout << "List has 2: " << (list.has(2) ? "true" : "false") << '\n';
    list.pop();

    for (int i = 0; i < 10; i++)
        list.append(i+1);

    std::cout << "Before insertion:\n";
    for (int i = 0; i < 10; i++)
        std::cout << list.get(i+1)->object << '\n';

    list.insert(12, 0);
    list.insert(12, 3);
    list.append(12);
    list.remove(6);
    list.prepend(15);
    std::cout << "After insertion and deletion:\n";
    for (int i = 0; i < list.length(); i++)
        std::cout << list.at(i)->object << '\n';

    std::cout << "After sorting:\n";
    list.sort();
    for (int i = 0; i < list.length(); i++)
    {
        ListNode<int>* element = list.at(i);
        std::cout << element->object << '\n';
    }

    std::cout << "Size: " << list.length() << '\n';
    std::cout << "Position of 2: " << list.position(2, 1) + 1 << '\n';

    LinkedList<int> list2;
    list2.append(100);
    list2.append(200);
    list.merge(list2);
    std::cout << "After merge:\n";
    std::cout << "Size: " << list.length() << '\n';

    LinkedList<int> list3;
    list3.sortAdd(4);
    list3.sortAdd(2);
    list3.sortAdd(1);
    std::cout << "Position: " << list3.sortPosition(3) << '\n';
    for (int i = 0; i < list3.length(); i++)
    {
        ListNode<int>* element = list3.at(i);
        std::cout << element->object << '\n';
    }

    return 0;
}
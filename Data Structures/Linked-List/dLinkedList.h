#pragma once
#include "dlist.h"
#include <stdexcept>

#define TEMP template<typename T>

enum SortCase
{
    SORTED,
    UNSORTED,
    INVALID
};

#define DLL DoubleLinkedList

TEMP
class DoubleLinkedList
{
   
    private:
        ListNode<T>* head;
        ListNode<T>* tail;
        int listLength;
        SortCase isSorted;

        void swap(ListNode<T>* first, ListNode<T>* second);
        void clear();
    
    public:
        DLL();
        DLL(const DLL& other);
        DLL<T>& operator=(const DLL& other);
        DLL(DLL&& other);
        DLL<T>& operator=(DLL&& other);
        ~DLL();

        size_t length();
        ListNode<T>* front();
        // Returns pointer to last node.
        ListNode<T>* back();

        // Add new node.

        void prepend(const T& object);
        void append(const T& object);
        void insert(const T& object, size_t position);

        // Find a node.

        int position(const T& object, size_t start = 0) const;

        // Remove node(s).

        T erase(size_t position);
        void remove(const T& object);
        T pop();
        void popn(size_t n);

        // Check for and retrieve nodes.

        bool has(const T& object) const;
        ListNode<T>* at(size_t position) const;
        ListNode<T>* get(const T& object) const;

        // Manage list.

        void sort(bool ascending = true);
        void merge(const DLL& other);

        // Manage sorted list.
        // All methods (except sorted()) will
        // assume the list object is already
        // sorted. They will not sort it if it isn't.

        // Check if it's sorted first.
        bool sorted(); // Will add ascending flag parameter later.
        void sortAdd(const T& object);
        int sortPosition(const T& object, size_t start = 0) const;
        bool sortHas(const T& object) const;
        void sortRemove(const T& object);

        // Make a copy of the list.
        friend DLL<T> copy(const DLL<T>& list);
};

TEMP
DLL<T>::DLL()
{
    head = nullptr;
    tail = nullptr;
    listLength = 0;
    isSorted = INVALID;
}

TEMP
void DLL<T>::clear()
{
    ListNode<T>* current = head;
    ListNode<T>* temp = nullptr;
    while (current != nullptr)
    {
        temp = current;
        current = current->next;
        delete temp;
    }

    listLength = 0;
    isSorted = INVALID;
}

TEMP
DLL<T>::DLL(const DLL<T>& other)
{
    int length = other.listLength;
    for (ListNode<T>* node = other.head; node != nullptr; node = node->next)
        this->append(node->object);
    
    // this->listLength = other.listLength;
    this->isSorted = other.isSorted;
}
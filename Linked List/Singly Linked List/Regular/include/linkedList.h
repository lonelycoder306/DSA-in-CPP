#pragma once
#include "../../General/list.h"
#include <stdexcept>

#define TEMP template<typename T>

enum SortCase
{
    SORTED,
    UNSORTED,
    INVALID
};

TEMP
class LinkedList
{
    private:
        ListNode<T>* head;
        int listLength;
        SortCase isSorted;

        void swap(ListNode<T>* first, ListNode<T>* second);
    
    public:
        LinkedList();
        ~LinkedList();

        int length();
        ListNode<T>* front();
        // Returns pointer to last node.
        ListNode<T>* back();

        // Add new node.

        void prepend(T object);
        void append(T object);
        void insert(T object, int position);

        // Find a node.

        int position(T object, int start = 0) const;

        // Remove node(s).

        T erase(int position);
        void remove(T object);
        T pop();
        void popn(int n);

        // Check for and retrieve nodes.

        bool has(T object) const;
        ListNode<T>* at(int position) const;
        ListNode<T>* get(T object) const;

        // Manage list.

        void sort(bool ascending = true);
        void merge(const LinkedList<T>& other);

        // Manage sorted list.
        // All methods (except sorted()) will
        // assume the list object is already
        // sorted. They will not sort it if it isn't.

        // Check if it's sorted first.
        bool sorted(); // Will add ascending flag parameter later.
        void sortAdd(T object);
        int sortPosition(T object, int start = 0) const;
        bool sortHas(T object) const;
        void sortRemove(T object);

        // Make a copy of the list.
        friend LinkedList<T> copy(const LinkedList<T>& list);
};

TEMP
LinkedList<T>::LinkedList()
{
    head = nullptr;
    listLength = 0;
    isSorted = INVALID;
}

TEMP
LinkedList<T>::~LinkedList()
{
    listLength = 0;
    ListNode<T>* current = head;
    ListNode<T>* temp = nullptr;
    while (current != nullptr)
    {
        temp = current;
        current = current->next;
        delete temp;
    }
}

TEMP
int LinkedList<T>::length()
{
    return listLength;
}

TEMP
ListNode<T>* LinkedList<T>::front()
{
    return head;
}

TEMP
ListNode<T>* LinkedList<T>::back()
{
    return this->at(listLength - 1);
}

TEMP
void LinkedList<T>::prepend(T object)
{
    ListNode<T>* newNode = new ListNode<T>;
    newNode->object = object;
    newNode->next = head;
    head = newNode;
    listLength++;
}

TEMP
void LinkedList<T>::append(T object)
{
    ListNode<T>* newNode = new ListNode<T>;
    newNode->object = object;
    newNode->next = nullptr;
    listLength++;

    if (head == nullptr) // Empty list.
    {
        head = newNode;
        return;
    }
    
    ListNode<T>* temp = head;
    while (temp->next != nullptr)
        temp = temp->next;
    
    temp->next = newNode;
}

TEMP
void LinkedList<T>::insert(T object, int position)
{
    if ((position < 0) ||
        ((position != 0) && (position >= listLength)))
            return; // Put error-handling here.

    // We will allow the user to use insert
    // on position 0 for an empty list.
    // No other position accepted if list is
    // empty.

    if (head == nullptr) // Empty list.
    {
        prepend(object);
        return;
    }
    
    ListNode<T>* newNode = new ListNode<T>;
    newNode->object = object;

    ListNode<T>* previous = nullptr;
    ListNode<T>* current = head;
    for (int i = 0; i < position; i++)
    {
        previous = current;
        current = current->next;
    }

    if (previous == nullptr) // We're adding it at the beginning.
        head = newNode;
    else
        previous->next = newNode;
    
    newNode->next = current;
    listLength++;
}

TEMP
int LinkedList<T>::position(T object, int start) const
{
    if ((start < 0) || (start >= listLength))
        return -1; // Put error-handling here.

    ListNode<T>* temp = head;
    for (int i = 0; i < start; i++)
        temp = temp->next;

    // Also handles head being a nullptr.
    int position = start;
    while (temp != nullptr)
    {
        if (temp->object == object)
            return position;
        temp = temp->next;
        position++;
    }
    
    return -1;
}

TEMP
T LinkedList<T>::erase(int position)
{
    if ((position < 0) || (position >= listLength))
        throw std::out_of_range("Invalid position value.");
    
    // Pointer to element before the one we want.
    ListNode<T>* previous = nullptr;
    ListNode<T>* current = head; // Pointer to the element we want.
    for (int i = 0; i < position; i++)
    {
        previous = current;
        current = current->next;
    }

    listLength--;

    if (previous == nullptr) // Removing the head.
    {
        ListNode<T>* temp = head;
        T element = head->object;
        head = head->next;
        delete temp;
        return element;
    }

    previous->next = current->next;
    T element = current->object;
    delete current;
    return element;
}

TEMP
void LinkedList<T>::remove(T object)
{
    int pos = position(object);
    if (pos != -1)
        erase(pos);
}

TEMP
T LinkedList<T>::pop()
{
    // erase() will automatically decrement
    // listLength.
    return erase(listLength - 1);
}

TEMP
void LinkedList<T>::popn(int n)
{
    while (n > 0)
    {
        pop();
        n--;
    }
}

TEMP
bool LinkedList<T>::has(T object) const
{
    return (position(object) != -1);
}

TEMP
ListNode<T>* LinkedList<T>::at(int position) const
{
    if ((position < 0) || (position >= listLength))
        return nullptr; // Put error-handling here.
    
    ListNode<T>* temp = head;
    for (int i = 0; i < position; i++)
        temp = temp->next;
    
    return temp;
}

TEMP
ListNode<T>* LinkedList<T>::get(T object) const
{
    int pos = position(object);
    if (pos == -1)
        return nullptr;
    
    return this->at(pos);
}

TEMP
void LinkedList<T>::swap(ListNode<T>* first, ListNode<T>* second)
{
    T tempObj = first->object;
    first->object = second->object;
    second->object = tempObj;
}

TEMP
// Using bubble sort.
void LinkedList<T>::sort(bool ascending)
{
    // We assume the list is currently unsorted
    // when this method is called, so we don't
    // check for the value of isSorted.

    if ((head == nullptr) || (head->next == nullptr))
    {
        // Empty list or only one element; nothing to sort.
        isSorted = SORTED;
        return;
    }
    
    bool ordered = false;

    while (!ordered)
    {
        ordered = true;
        ListNode<T>* first = head;
        ListNode<T>* second = head->next;

        for (int i = 0; i < listLength - 1; i++)
        {
            if (ascending)
            {
                if (first->object > second->object)
                {
                    swap(first, second);
                    ordered = false;
                }
            }
            else
            {
                if (first->object < second->object)
                {
                    swap(first, second);
                    ordered = false;
                }
            }

            first = second;
            second = second->next;
        }
    }

    isSorted = SORTED;
}

TEMP
void LinkedList<T>::merge(const LinkedList<T>& other)
{
    int length = other.listLength;
    // We can't simply connect the end node
    // for this list to the head node of
    // the other, since they would then have
    // duplicate pointers, leading to double-freeing
    // when both list objects' destructors are called.
    for (int i = 0; i < length; i++)
        this->append(other.at(i)->object);
    // Cannot know if the merged list is still sorted.
    isSorted = INVALID;
}

TEMP
bool LinkedList<T>::sorted()
{
    // If previous state is known, return it.
    if (isSorted == SORTED || isSorted == UNSORTED)
        return (isSorted == SORTED); // True if sorted, false otherwise.
    
    if ((head == nullptr) || (head->next == nullptr))
    {
        isSorted = SORTED;
        return true;
    }
    
    ListNode<T>* first = head;
    ListNode<T>* second = head->next;

    for (int i = 0; i < listLength - 1; i++)
    {
        if (first->object > second->object)
        {
            isSorted = UNSORTED;
            return false;
        }

        first = second;
        second = second->next;
    }

    isSorted = SORTED;
    return true;
}

TEMP
void LinkedList<T>::sortAdd(T object)
{
    ListNode<T>* temp = head;
    int position = 0;
    while (temp != nullptr)
    {
        if (temp->object > object)
            break;
        temp = temp->next;
        position++;
    }
    if (position != listLength)
        insert(object, position);
    else
        append(object);
}

TEMP
int LinkedList<T>::sortPosition(T object, int start) const
{
    if ((start < 0) || (start >= listLength))
        return -1; // Throw error?
    
    ListNode<T>* temp = head;
    for (int i = 0; i < start; i++)
        temp = temp->next;

    int position = start;
    while (temp != nullptr)
    {
        if (temp->object == object)
            return position;
        if (temp->object > object) // It's not there.
            break;
        temp = temp->next;
        position++;
    }

    return -1;
}

TEMP
bool LinkedList<T>::sortHas(T object) const
{
    return (sortPosition(object) != -1);
}

TEMP
void LinkedList<T>::sortRemove(T object)
{
    int pos = sortPosition(object);
    if (pos != -1)
        erase(pos);
}

TEMP
LinkedList<T> copy(const LinkedList<T>& list)
{
    int length = list.listLength;
    LinkedList<T> newList;

    for (int i = 0; i < length; i++)
        // Uses copy constructor(?) for T object.
        newList.append(list.at(i)->object);
    
    newList.isSorted = list.isSorted;
    
    return newList;
}
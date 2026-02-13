#pragma once
#include "list.h"
#include <cstdint>
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
        size_t listLength;
        SortCase isSorted;

        void swap(ListNode<T>* first, ListNode<T>* second);
    
    public:
        LinkedList();
        LinkedList(const LinkedList& other);
        LinkedList& operator=(const LinkedList& other);
        LinkedList(LinkedList&& other);
        LinkedList& operator=(LinkedList&& other);
        ~LinkedList();

        bool operator==(const LinkedList& other);

        size_t length();
        ListNode<T>* front();
        // Returns pointer to last node.
        ListNode<T>* back();
        void clear(void (*func)(T obj));

        // Add new node.

        void prepend(const T& object);
        void append(const T& object);
        void insert(const T& object, size_t position);

        // Find a node.

        size_t position(const T& object, size_t start = 0) const;

        // Remove node(s).

        T erase(size_t position);
        void remove(const T& object);
        T pop();
        void popn(size_t n);

        // Check for and retrieve nodes.

        bool has(T object) const;
        ListNode<T>* at(size_t position) const;
        ListNode<T>* get(const T& object) const;

        // Manage list.

        void sort(bool ascending = true);
        void reverse();
        void merge(const LinkedList& other);

        // Manage sorted list.
        // All methods (except sorted()) will
        // assume the list object is already
        // sorted. They will not sort it if it isn't.

        // Check if it's sorted first.
        bool sorted(); // Will add ascending flag parameter later.
        void sortAdd(const T& object);
        size_t sortPosition(const T& object, size_t start = 0) const;
        bool sortHas(const T& object) const;
        void sortRemove(const T& object);

        // Make a copy of the list.
        template<typename U>
        friend LinkedList<U> copy(const LinkedList<U>& list);

        class iterator
        {
            private:
                ListNode<T>* ptr;
            
            public:
                iterator(ListNode<T>* ptr);
                iterator(const iterator& other);
                iterator& operator=(const iterator& other);

                T& operator*() const;
                iterator& operator++();
                iterator operator++(int);
                bool operator==(const iterator& other);
                bool operator!=(const iterator& other);
        };

        class const_iterator
        {
            private:
                const ListNode<T>* ptr;
            
            public:
                const_iterator(const ListNode<T>* ptr);
                const_iterator(const const_iterator& other);
                const_iterator& operator=(const const_iterator& other);

                const T& operator*() const;
                const_iterator& operator++();
                const_iterator operator++(int);
                bool operator==(const const_iterator& other) const;
                bool operator!=(const const_iterator& other) const;
        };

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator cbegin() const;
        const_iterator cend() const;
};

TEMP
LinkedList<T>::LinkedList() :
    head(nullptr), listLength(0),
    isSorted(INVALID) {}

TEMP
void LinkedList<T>::clear(void (*func)(T obj))
{
    ListNode<T>* current = head;
    ListNode<T>* temp = nullptr;
    while (current != nullptr)
    {
        temp = current;
        if (func != nullptr) (*func)(temp->object);
        current = current->next;
        delete temp;
    }

    listLength = 0;
    isSorted = INVALID;
}

TEMP
LinkedList<T>::LinkedList(const LinkedList<T>& other)
{
    for (ListNode<T>* node = other.head; node != nullptr; node = node->next)
        this->append(node->object);
    
    // this->listLength = other.listLength;
    this->isSorted = other.isSorted;
}

TEMP
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
    if (this != &other)
    {
        this->clear();
        
        for (ListNode<T>* node = other.head; node != nullptr; node = node->next)
            this->append(node->object);

        // this->listLength = other.listLength;
        this->isSorted = other.isSorted;
    }

    return *this;
}

TEMP
LinkedList<T>::LinkedList(LinkedList<T>&& other)
{
    this->head = other.head;
    this->listLength = other.listLength;
    this->isSorted = other.isSorted;

    other.head = nullptr;
    other.listLength = 0;
    other.isSorted = INVALID;
}

TEMP
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other)
{
    this->clear();
    
    this->head = other.head;
    this->listLength = other.listLength;
    this->isSorted = other.isSorted;

    other.head = nullptr;
    other.listLength = 0;
    other.isSorted = INVALID;

    return *this;
}

TEMP
LinkedList<T>::~LinkedList()
{
    this->clear(nullptr);
}

TEMP
bool LinkedList<T>::operator==(const LinkedList<T>& other)
{
    if (this->listLength != other.listLength) return false;

    ListNode<T>* thisTemp = this->head;
    ListNode<T>* otherTemp = other.head;

    while ((thisTemp != nullptr)/* && (otherTemp != nullptr)*/)
    {
        if (thisTemp->object != otherTemp->object)
            return false;
        thisTemp = thisTemp->next;
        otherTemp = otherTemp->next;
    }

    return true;
}

TEMP
size_t LinkedList<T>::length()
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
    // Temporarily.
    // We can mimic std::list and make
    // the last node a dummy sentinel node.
    return nullptr;
}

TEMP
void LinkedList<T>::prepend(const T& object)
{
    ListNode<T>* newNode = new ListNode<T>;
    newNode->object = object;
    newNode->next = head;
    head = newNode;
    listLength++;
}

TEMP
void LinkedList<T>::append(const T& object)
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
void LinkedList<T>::insert(const T& object, size_t position)
{
    if ((position != 0) && (position >= listLength))
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
size_t LinkedList<T>::position(const T& object, size_t start) const
{
    if (start >= listLength)
        return SIZE_MAX; // Put error-handling here.

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
T LinkedList<T>::erase(size_t position)
{
    if (position >= listLength)
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
void LinkedList<T>::remove(const T& object)
{
    int pos = position(object);
    if (pos != SIZE_MAX)
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
void LinkedList<T>::popn(size_t n)
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
    return (position(object) != SIZE_MAX);
}

TEMP
ListNode<T>* LinkedList<T>::at(size_t position) const
{
    if (position >= listLength)
        return nullptr; // Put error-handling here.
    
    ListNode<T>* temp = head;
    for (int i = 0; i < position; i++)
        temp = temp->next;
    
    return temp;
}

TEMP
ListNode<T>* LinkedList<T>::get(const T& object) const
{
    int pos = position(object);
    if (pos == SIZE_MAX)
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
void LinkedList<T>::reverse()
{
    if (listLength <= 1) return;

    ListNode<T>* first = head;
    ListNode<T>* second = head->next;

    // Disconnect next node from head (the head
    // node will become the last node after we're
    // done reversing).
    head->next = nullptr;

    while (second != nullptr)
    {
        ListNode<T>* temp = second->next;
        second->next = first;
        first = second;
        second = temp;
    }

    // Reassign the head to the new first node.
    head = first;
}

TEMP
void LinkedList<T>::merge(const LinkedList<T>& other)
{
    // We can't simply connect the end node
    // for this list to the head node of
    // the other, since they would then have
    // duplicate pointers, leading to double-freeing
    // when both list objects' destructors are called.

    for (ListNode<T>* node = other.head; node != nullptr; node = node->next)
        this->append(node->object);

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
void LinkedList<T>::sortAdd(const T& object)
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
size_t LinkedList<T>::sortPosition(const T& object, size_t start) const
{
    if (start >= listLength)
        return SIZE_MAX; // Throw error?
    
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
bool LinkedList<T>::sortHas(const T& object) const
{
    return (sortPosition(object) != SIZE_MAX);
}

TEMP
void LinkedList<T>::sortRemove(const T& object)
{
    int pos = sortPosition(object);
    if (pos != SIZE_MAX)
        erase(pos);
}

TEMP
LinkedList<T> copy(const LinkedList<T>& list)
{
    LinkedList<T> newList;

    for (ListNode<T>* node = list.head; node != nullptr; node = node->next)
        // Uses copy constructor(?) for T object.
        newList.append(node->object);
    
    newList.isSorted = list.isSorted;

    return newList;
}

// Iterator implementation.

#define LLIter LinkedList<T>::iterator

TEMP
LLIter::iterator(ListNode<T>* ptr) :
    ptr(ptr) {}

TEMP
LLIter::iterator(const LLIter& other) :
    ptr(other.ptr) {}

TEMP
typename LLIter& LLIter::operator=(const LLIter& other)
{
    this->ptr = other.ptr;
    return *this;
}

TEMP
T& LLIter::operator*() const
{
    return ptr->object;
}

TEMP
typename LLIter& LLIter::operator++()
{
    ptr = ptr->next;
    return *this;
}

TEMP
typename LLIter LLIter::operator++(int n)
{
    LLIter temp = *this;
    ptr = ptr->next;
    return temp;
}

TEMP
bool LLIter::operator==(const LLIter& other)
{
    return (this->ptr == other.ptr);
}

TEMP
bool LLIter::operator!=(const LLIter& other)
{
    return (this->ptr != other.ptr);
}

TEMP
typename LLIter LinkedList<T>::begin()
{
    return iterator(front());
}

TEMP
typename LLIter LinkedList<T>::end()
{
    return iterator(back());
}

// Const iterator implementation.

#define constListIter LinkedList<T>::const_iterator

TEMP
constListIter::const_iterator(const ListNode<T>* ptr) :
    ptr(ptr) {}

TEMP
constListIter::const_iterator(const const_iterator& other) :
    ptr(other.ptr) {}

TEMP
typename constListIter& constListIter::operator=(const constListIter& other)
{
    this->ptr = other.ptr;
}

TEMP
const T& constListIter::operator*() const
{
    return ptr->object;
}

TEMP
typename constListIter& constListIter::operator++()
{
    ptr = ptr->next;
    return *this;
}

TEMP
typename constListIter constListIter::operator++(int n)
{
    constListIter temp = *this;
    ptr = ptr->next;
    return temp;
}

TEMP
bool constListIter::operator==(const constListIter& other) const
{
    return (this->ptr == other.ptr);
}

TEMP
bool constListIter::operator!=(const constListIter& other) const
{
    return (this->ptr != other.ptr);
}

TEMP
typename constListIter LinkedList<T>::begin() const
{
    return const_iterator(front());
}

TEMP
typename constListIter LinkedList<T>::end() const
{
    return const_iterator(back());
}

TEMP
typename constListIter LinkedList<T>::cbegin() const
{
    return const_iterator(front());
}

TEMP
typename constListIter LinkedList<T>::cend() const
{
    return const_iterator(back());
}
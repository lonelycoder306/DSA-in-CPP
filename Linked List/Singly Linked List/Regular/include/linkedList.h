#include "../../General/list.h"

#define TEMP template<typename T>

TEMP
class LinkedList
{
    private:
        ListNode<T>* head;
        int listLength;

        void swap(ListNode<T>* first, ListNode<T>* second);
    
    public:
        LinkedList();
        ~LinkedList();

        int length();
        ListNode<T>* front();
        ListNode<T>* end();

        void prepend(T object);
        void append(T object);
        void insert(T object, int position);
        void remove(int position);
        int position(T object, int start = 0);
        bool has(T object);
        ListNode<T>* at(int position);
        ListNode<T>* get(T object);
        void sort(bool ascending = true);
        void merge(LinkedList<T>& other);

        friend LinkedList<T> copy(const LinkedList<T>& list);
};

TEMP
LinkedList<T>::LinkedList()
{
    head = nullptr;
    listLength = 0;
}

TEMP
LinkedList<T>::~LinkedList()
{
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
ListNode<T>* LinkedList<T>::end()
{
    return this->at(length() - 1);
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
    if ((position < 0) || (position >= listLength))
        return; // Put error-handling here.

    // Not needed because of above condition.
    // Remove?
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

    if (previous != nullptr)
        previous->next = newNode;
    else // We're adding it at the beginning.
        head = newNode;
    
    newNode->next = current;
    listLength++;
}

TEMP
void LinkedList<T>::remove(int position)
{
    if ((position < 0) || (position >= listLength))
        return; // Put error-handling here.
    
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
        head = head->next;
        return;
    }

    previous->next = current->next;
    delete current;
}

TEMP
int LinkedList<T>::position(T object, int start)
{
    if ((start < 0) || (start >= listLength))
        return -1; // Put error-handling here.

    ListNode<T>* temp = head;
    for (int i = 0; i < start; i++)
        temp = temp->next;

    // Also handles head being a nullptr.
    int position = start - 1;
    while (temp != nullptr)
    {
        position++;
        if (temp->object == object)
            return position;
        temp = temp->next;
    }
    
    return -1;
}

TEMP
bool LinkedList<T>::has(T object)
{
    ListNode<T>* temp = head;
    while (temp != nullptr)
    {
        if (temp->object == object)
            return true;
        temp = temp->next;
    }
    
    return false;
}

TEMP
ListNode<T>* LinkedList<T>::at(int position)
{
    if ((position < 0) || (position >= listLength))
        return nullptr; // Put error-handling here.
    
    ListNode<T>* temp = head;
    for (int i = 0; i < position; i++)
        temp = temp->next;
    
    return temp;
}

TEMP
ListNode<T>* LinkedList<T>::get(T object)
{
    int pos = position(object);
    if (pos == -1)
        return nullptr;
    
    ListNode<T>* temp = head;
    for (int i = 0; i < pos; i++)
        temp = temp->next;
    
    return temp;
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
    if ((head == nullptr) || (head->next == nullptr))
        return; // Empty list or only one element; nothing to sort.
    
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
                if (first->object < second->object) // Problem with this one.
                {
                    swap(first, second);
                    ordered = false;
                }
            }

            first = second;
            second = second->next;
        }
    }
}

TEMP
void LinkedList<T>::merge(LinkedList<T>& other)
{
    end()->next = other.head;
    this->listLength += other.listLength;
}

TEMP
LinkedList<T> copy(const LinkedList<T>& list)
{
    int length = list.length();
    LinkedList<T> newList;

    for (int i = 0; i < length; i++)
        // Uses copy constructor(?) for T object.
        newList.append(list.at(i)->object);
    
    return newList;
}
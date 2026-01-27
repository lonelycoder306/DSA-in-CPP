template<typename T>
struct ListNode
{
    T object;
    ListNode<T>* prev;
    ListNode<T>* next;

    ListNode();
};

template<typename T>
ListNode<T>::ListNode() :
    prev(nullptr), next(nullptr) {} // Let default constructor for T run.
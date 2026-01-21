template<typename T>
struct ListNode
{
    T object;
    ListNode<T>* next;

    ListNode();
};

template<typename T>
ListNode<T>::ListNode() :
    next(nullptr) {} // Let default constructor for T run.
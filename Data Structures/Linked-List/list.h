#include <type_traits>

template<typename T>
struct ListNode
{
    static_assert(std::is_default_constructible_v<T>,
        "Node object type is not default constructible.");
    
    T object;
    ListNode<T>* next;

    ListNode();
};

template<typename T>
ListNode<T>::ListNode() :
    next(nullptr) {} // Let default constructor for T run.
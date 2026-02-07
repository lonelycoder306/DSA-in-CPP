#ifdef BINARY

template<typename T>
int search(T* elems, int count, T find)
{
    int begin = 0;
    int end = count - 1;
    while (end >= begin)
    {
        int mid = begin + (end - begin)/2;
        if (elems[mid] == find)
            return mid;
        else if (elems[mid] > find)
            end = mid - 1;
        else
            begin = mid + 1;
    }

    return -1;
}

#endif
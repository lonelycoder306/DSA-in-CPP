#ifdef SELECTION

template<typename T>
static inline int findMin(T* elems, int start, int count)
{
    int index = start;
    T min = elems[start];
    for (int i = start; i < count; i++)
    {
        if (elems[i] < min)
        {
            min = elems[i];
            index = i;
        }
    }

    return index;
}

template<typename T>
inline void sort(T* elems, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        int index = findMin(elems, i, count);
        if (index != i)
        {
            T temp = elems[i];
            elems[i] = elems[index];
            elems[index] = temp;
        }
    }
}

#endif
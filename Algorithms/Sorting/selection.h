#ifdef SELECTION

static inline int findMin(int* elems, int start, int count)
{
    int index = start;
    int min = elems[start];
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

inline void sort(int* elems, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        int index = findMin(elems, i, count);
        if (index != i)
        {
            int temp = elems[i];
            elems[i] = elems[index];
            elems[index] = temp;
        }
    }
}

#endif
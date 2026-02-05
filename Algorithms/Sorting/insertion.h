#ifdef INSERTION

static inline void insert(int* elems, int start, int index)
{
    int temp = elems[index];
    for (int i = start + 1; i <= index; i++)
        elems[i] = elems[i - 1];
    elems[start] = temp;
}

inline void sort(int* elems, int count)
{
    for (int i = 1; i < count; i++)
    {
        if (elems[i - 1] <= elems[i]) continue;
        int j = i - 1;
        while ((elems[j] > elems[i]) && (j >= 0))
            j--;
        insert(elems, j + 1, i);
    }
}

#endif
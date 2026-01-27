template<typename T>
int range(T* elems, int count, T& min)
{
    T max = elems[0];
    min = elems[0];

    for (int i = 0; i < count; i++)
    {
        if (elems[i] > max) max = elems[i];
        else if (elems[i] < min) min = elems[i];
    }

    return (max - min + 1);
}

template<typename T>
inline void sort(T*& elems, int count)
{
    T min;
    int r = range(elems, count, min);
    int* temp = new int[r];
    for (int i = 0; i < r; i++)
        temp[i] = 0;
    for (int i = 0; i < count; i++)
        temp[elems[i] - min]++;
    for (int i = 0; i < r - 1; i++)
        temp[i + 1] += temp[i];

    T* newElems = new T[count];
    for (int i = 0; i < count; i++)
    {
        newElems[temp[elems[i] - min] - 1] = elems[i];
        temp[elems[i] - min]--;
    }

    delete[] temp;
    delete[] elems;
    elems = newElems;
}
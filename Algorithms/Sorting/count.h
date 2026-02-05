#ifdef COUNT

#include <cstring>

int range(int* elems, int count, int& min)
{
    int max = elems[0];
    min = elems[0];

    for (int i = 0; i < count; i++)
    {
        if (elems[i] > max) max = elems[i];
        else if (elems[i] < min) min = elems[i];
    }

    return (max - min + 1);
}

inline void sort(int*& elems, int count)
{
    int min;
    int r = range(elems, count, min);
    int* temp = new int[r];
    memset(temp, 0, r * sizeof(int));
    for (int i = 0; i < count; i++)
        temp[elems[i] - min]++;
    for (int i = 0; i < r - 1; i++)
        temp[i + 1] += temp[i];

    int* newElems = new int[count];
    for (int i = 0; i < count; i++)
    {
        newElems[temp[elems[i] - min] - 1] = elems[i];
        temp[elems[i] - min]--;
    }

    delete[] temp;
    delete[] elems;
    elems = newElems;
}

#endif
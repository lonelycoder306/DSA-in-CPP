#ifdef QUICK

inline void sort(int* elems, int count)
{
    if (count < 2) return;

    int pivot = elems[count - 1];
    int i = -1;
    for (int j = 0; j < count - 1; j++)
    {
        if (elems[j] < pivot)
        {
            i++;
            if (i != count - 1)
            {
                int temp = elems[i];
                elems[i] = elems[j];
                elems[j] = temp;
            }
        }
    }

    int temp = elems[count - 1];
    elems[count - 1] = elems[i + 1];
    elems[i + 1] = temp;
    sort(elems, i + 1);
    sort(&elems[i + 2], count - i - 2);
}

#endif
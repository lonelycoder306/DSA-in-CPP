template<typename T>
inline void sort(T* elems, int count)
{
    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (int i = 0; i < count - 1; i++)
        {
            if (elems[i] > elems[i + 1])
            {
                int temp = elems[i];
                elems[i] = elems[i + 1];
                elems[i + 1] = temp;
                sorted = false;
            }
        }
    }
}
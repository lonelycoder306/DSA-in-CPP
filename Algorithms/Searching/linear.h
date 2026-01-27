template<typename T>
int search(T* elems, int count, T find)
{
    for (int i = 0; i < count; i++)
    {
        if (elems[i] == find)
            return i;
    }

    return -1;
}
#ifdef RADIX

#include <cmath>
#include <cstring>

int getDigits(int elem)
{
    if (elem == 0) return 1;

    int digits = 0;
    while (elem > 0)
    {
        elem /= 10;
        digits++;
    }

    return digits;
}

int maxDigits(int* elems, int count)
{
    int max = elems[0];
    for (int i = 1; i < count; i++)
    {
        if (elems[i] > max) max = elems[i];
    }

    return getDigits(max);
}

void combineBuckets(int* buckets, int* elems, int count, int indices[])
{
    int pos = 0;
    for (int i = 0; i < 10; i++)
    {
        int max = indices[i];
        for (int j = 0; j < max; j++)
            elems[pos++] = buckets[(count + 1) * i + j];
    }
}

inline void sort(int* elems, int count)
{
    constexpr int numDigits = 10;
    // count + 1 allows us to add a terminating
    // -1 at the end of each, while still possibly
    // storing the entire array in each bucket.
    int* buckets = new int[numDigits * (count + 1)];
    int size = numDigits * (count + 1);
    memset(buckets, -1, numDigits * (count + 1) * sizeof(int));
    int length = maxDigits(elems, count);

    for (int i = 0; i < length; i++)
    {
        int max = static_cast<int>(pow(10, i));
        int indices[10] = {0};
        for (int j = 0; j < count; j++)
        {
            if (elems[j] < max)
                buckets[indices[0]++] = elems[j];
            else
            {
                int digit = (elems[j] % (max * 10));
                digit /= max;
                buckets[digit * (count + 1) + indices[digit]++] = elems[j];
            }
        }

        combineBuckets(buckets, elems, count, indices);
        memset(buckets, -1, numDigits * (count + 1) * sizeof(int));
    }
}

#endif
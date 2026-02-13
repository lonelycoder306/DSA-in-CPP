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

void combineBuckets(int* buckets, int* elems, int count, int counters[])
{
    int pos = 0;
    for (int i = 0; i < 10; i++)
    {
        int max = counters[i];
        for (int j = 0; j < max; j++)
            elems[pos++] = buckets[count * i + j];
    }
}

inline void sort(int* elems, int count)
{
    constexpr int numDigits = 10;
    int* buckets = new int[numDigits * count];
    int size = numDigits * count;
    int length = maxDigits(elems, count);

    for (int i = 0; i < length; i++)
    {
        int max = static_cast<int>(pow(10, i));
        int counters[10] = {0};
        for (int j = 0; j < count; j++)
        {
            if (elems[j] < max)
                buckets[counters[0]++] = elems[j];
            else
            {
                int digit = (elems[j] % (max * 10));
                digit /= max;
                buckets[digit * count + counters[digit]++] = elems[j];
            }
        }

        combineBuckets(buckets, elems, count, counters);
    }
}

#endif
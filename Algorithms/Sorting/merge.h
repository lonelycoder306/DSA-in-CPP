#ifdef MERGE

#include <cstring>

static inline void merge(int* elems, int left, int mid, int right)
{
    int lSize = mid - left + 1;
    int rSize = right - mid;

    int* leftNums = new int[lSize];
    int* rightNums = new int[rSize];

    memcpy(leftNums, elems + left, lSize * sizeof(int));
    memcpy(rightNums, elems + mid + 1, rSize * sizeof(int));

    int i = 0, j = 0, pos = left;
    while ((i < lSize) && (j < rSize))
    {
        if (leftNums[i] <= rightNums[j])
            elems[pos++] = leftNums[i++];
        else
            elems[pos++] = rightNums[j++];
    }

    memcpy(elems + pos, leftNums + i, (lSize - i) * sizeof(int));
    memcpy(elems + pos, rightNums + j, (rSize - j) * sizeof(int));
}

// To keep the interface below consistent.
static inline void mergeSort(int* elems, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left)/2;
        mergeSort(elems, left, mid);
        mergeSort(elems, mid + 1, right);
        merge(elems, left, mid, right);
    }
}

inline void sort(int* elems, int count)
{
    mergeSort(elems, 0, count - 1);
}

#endif
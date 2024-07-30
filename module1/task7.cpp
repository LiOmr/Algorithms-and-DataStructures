/*
Задача 7 Вариант 2: LSD для long long.
Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 10^6.
Отсортировать массив методом поразрядной сортировки LSD по байтам.
*/

#include <assert.h>
#include <cstring>
#include <iostream>

#define BYTE 8
#define BYTE_MASK 255 

template<typename T, typename TKey>
void CountingSort(T* array, int len, TKey getKey) {
    unsigned long long max = getKey(array[0]);
    for(int i = 1; i < len; ++i) {
        if (max < getKey(array[i])) {
            max = getKey(array[i]);
        }
    }

    int *countArray = new int[max + 1];
    memset(countArray, 0, (max + 1) * sizeof(int));

    for(int i = 0; i < len; ++i)
        ++countArray[getKey(array[i])];

    for(unsigned int i = 1; i <= max; ++i) {
        countArray[i] += countArray[i - 1];
    }

    T* tmpArray = new T[len];
    for(int i = len - 1; i >= 0; --i) {
        tmpArray[--countArray[getKey(array[i])]] = array[i];
    }
    delete[] countArray;

    memcpy(array, tmpArray, len * sizeof(T));

    delete[] tmpArray;
}

template<typename T>
void SortLSD(T *array, int len) {
    for (unsigned int i = 0; i < sizeof(T); ++i) {
        CountingSort(array, len, [i](T number) { return (number >> BYTE * i) & BYTE_MASK; });
    }
}

int main()
{
    int size = 0;
    std::cin >> size;
    assert(size > 0);

    unsigned long long *array = new unsigned long long[size];

    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    SortLSD(array, size);

    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }

    delete [] array;
    return 0;
}

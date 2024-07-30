/* Дано множество целых чисел из [0..10^9] размера n.
Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
10%  перцентиль
медиана
90%  перцентиль

Требования: к дополнительной памяти: O(n).
Среднее время работы: O(n)
Должна быть отдельно выделенная функция partition.
Рекурсия запрещена.
Решение должно поддерживать передачу функции сравнения снаружи.


Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении. Описание для случая прохода от начала массива к концу:
Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

6_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу. */


#include <iostream>
#include <assert.h>
#define PERCENTAGE 100


template<class T>
class IsLessCompareDefault
{
public:
    bool operator()(const T& left, const T& right) { return left < right; }
};

template<typename T>
void Swap (T& left, T& right)
{
    T temp = left;
    left = right;
    right = temp;
}

template<class T, class Compare = IsLessCompareDefault<T>>
size_t MedianOfThree(int* arr, const size_t& left, const size_t& right, Compare isLess = IsLessCompareDefault<T>()) {
    size_t middle = (left + right) / 2;
    if (isLess(arr[right], arr[left])) {
        Swap(arr[left], arr[right]);
    }
    if (isLess(arr[middle], arr[left])) {
        Swap(arr[middle], arr[left]);
    }
    if (isLess(arr[right], arr[middle])) {
        Swap(arr[right], arr[middle]);
    }
    return middle;
}


template<class T, class Compare = IsLessCompareDefault<T>>
size_t Partition(int* arr, const size_t& left, const size_t& right, Compare isLess = IsLessCompareDefault<T>()) {
    size_t pivotIndex = MedianOfThree<T>(arr, left, right, isLess);
    T pivot = arr[pivotIndex];
    Swap(arr[pivotIndex], arr[right]);
    size_t i = left;
    size_t j = left;
    while (true) {
        while (i < right && (isLess(arr[i], pivot) || arr[i] == pivot)) {
            ++i;
        }
        while (((j < right) && (isLess(pivot, arr[j]))) || (j < i)) {
            ++j;
        }
        if (j < right && isLess(arr[j], arr[i])) {
            Swap(arr[i], arr[j]);
            ++i;
            ++j;
        } else {
            Swap(arr[i], arr[right]);
            return i;
        }
    }
}

template<class T, class Compare = IsLessCompareDefault<T>>
int FindKStatistic(int* arr, size_t left, size_t right, const size_t& k, Compare isLess = IsLessCompareDefault<T>()) {
    assert(k >= 0 && k <= right);
    size_t pivotPosition = Partition<T>(arr, left, right, isLess);
    while (pivotPosition != k) {
        if (k >= pivotPosition) {
            left = pivotPosition + 1;
        } else {
            right = pivotPosition - 1;
        }
        pivotPosition = Partition<T>(arr, left, right, isLess);
    }
    return arr[pivotPosition];
}

int main() {
    IsLessCompareDefault<int> isLess;
    int arrSize = 0;
    std::cin >> arrSize;
    assert(arrSize >= 0);
    int* array = new int[arrSize];
    for (int i = 0; i < arrSize; ++i) {
        std::cin >> array[i];
    }

    const int percentiles[] = {10, 50, 90};
    for (int percentile : percentiles) {
        std::cout << FindKStatistic<int>(array, 0, arrSize - 1, percentile * arrSize / PERCENTAGE, isLess) << std::endl;
    }

    delete[] array;
    return 0;
}
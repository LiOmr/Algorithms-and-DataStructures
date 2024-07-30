/* 2_3. Даны два массива неубывающих целых чисел, упорядоченные по возрастанию. A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A.. В процессе поиска очередного элемента B[i] в массиве A
пользуйтесь результатом поиска элемента B[i-1].
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k
с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.

n, k ≤ 10000. */

#include <iostream>

using namespace std;


int intersection(const int* array, const size_t& size, const int& target, size_t& index) {  //exponential + binary search

    size_t left = index, right = left + 1;
    while (right < size && array[right] < target) {
        left = right;
        right *= 2;
    }

    if (right >= size) {
        right = size - 1;
    }

    size_t middle;

    while (left < right) {

        middle = (left + right) / 2;
        if (array[middle] < target) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    index = left;
    return (left == size || array[left] != target)? -1 : left;

}


int main() {

    size_t len_a = 0;
    size_t len_b = 0;
    cin >> len_a >> len_b ;

    int* A = new int[len_a];
    for (size_t i = 0; i < len_a; ++i) {
        cin >> A[i];
    }

    int* B = new int [len_b];
    for (size_t i = 0; i < len_b; ++i) {
        cin >> B[i];
    }

    int elem = 0;
    size_t index = 0;

    for (size_t i = 0; i < len_b; ++i) {

        elem = intersection(A, len_a, B[i], index);

        if (elem != -1) {
            cout << A[elem] << ' ';
        }

    }

    delete[] A;
    delete[] B;

    return 0;
}

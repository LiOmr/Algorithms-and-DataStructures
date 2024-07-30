/* 5.1 Реклама.
 * В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода покупателей (два целых числа).
 * Каждому покупателю необходимо показать минимум 2 рекламы. Рекламу можно транслировать только в целочисленные моменты времени.
 * Покупатель может видеть рекламу от момента прихода до момента ухода из магазина. В каждый момент времени может показываться только одна реклама.
 * Считается, что реклама показывается мгновенно. Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
 * Требуется определить минимальное число показов рекламы. */

#include <iostream>
#include <cassert>

struct Visitor {
    int entry;
    int exit;
    Visitor() = default;
    Visitor(int entry_, int exit_): entry(entry_), exit(exit_) {}

};


bool operator<(const Visitor& first, const Visitor& second) {
    if (first.exit == second.exit) {
        return first.entry <= second.entry;
    }
    return first.exit > second.exit;
}

template <typename T>
struct sortComparator {
    bool operator() (const T& first, const T& second) const{
        return first < second;
    }
};


template<typename T, typename Comparator = sortComparator<T>>
void merge(T array[], int left, int mid, int right, Comparator cmp = Comparator()) {
  if (left >= right || mid >= right) return;

  int iLeftSubarr = left;
  int iRightSubarr = mid;

  T *result = new T[right - left];
  int pos = 0;

  while ((iLeftSubarr < mid) && (iRightSubarr < right)) {
    if (cmp(array[iLeftSubarr], array[iRightSubarr])) {
      result[pos] = array[iRightSubarr++];
    } else {
      result[pos] = array[iLeftSubarr++];
    }

    pos++;
  }

  if (iLeftSubarr == mid) {
    for (int i = iRightSubarr; i < right; ++i) {
      result[pos++] = array[i];
    }
  } else {
    for (int i = iLeftSubarr; i < mid; ++i) {
      result[pos++] = array[i];
    }
  }

  for (int i = 0; i < right - left; ++i) {
    array[left + i] = result[i];
  }

  delete[] result;
}


template <typename T, typename Comparator = sortComparator<T>>
void sort(T *array, size_t size, Comparator cmp = Comparator())
{
    if (size == 0) return;

    size_t step = 1;
      while (step < size) {
        size_t left = 0;

        while (left < size && left + step < size) {
          int mid = left + step;

          int right = 0;
          if ((left + step * 2) > size) {
              right = size;
          } else {
              right = mid + step;
          }

          merge(array, left, mid, right, cmp);
          left += step * 2;
        }

        step *= 2;
      }
}

template <typename T>
int advertisementCount(T *array, size_t size) {
    int advCount = 0;
    int first = -1;
    int second = -1;

    for (size_t i = 0; i < size; ++i) {
        if (array[i].entry > second) {
            advCount += 2;
            first = array[i].exit - 1;
            second = array[i].exit;
        }  else if (array[i].entry == second) {
            advCount++;
            first = second;
            second = array[i].exit;
        } else if (array[i].entry > first && array[i].exit > second) {
            advCount++;
            first = second;
            second = array[i].exit;
        } else if (array[i].entry > first && array[i].entry < second) {
            advCount++;
            first = array[i].exit;
        }
    }
    return advCount;
}



int main() {
    int visitorsCount = 0;
    std::cin >> visitorsCount;
    assert(visitorsCount >= 0);
    Visitor* visitors = new Visitor[visitorsCount];

    int entry, exit = 0;
    for (int i = 0; i < visitorsCount; ++i) {
        std::cin >> entry >> exit;
        assert(entry >= 0 && exit >= 0);
        visitors[i] = Visitor(entry, exit);
    }

    sort(visitors, visitorsCount, sortComparator<Visitor>());

    int result = advertisementCount(visitors, visitorsCount);
    std::cout << result;

    delete[] visitors;

    return 0;
}

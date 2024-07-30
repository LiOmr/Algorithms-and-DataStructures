/* 4.3 Планировщик процессов
В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется :
приоритетом P
временем, которое он уже отработал t
временем, которое необходимо для завершения работы процесса T
Планировщик процессов выбирает процесс с минимальным значением P* (t + 1), выполняет его время P и кладет обратно в очередь процессов.
Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол - во переключений процессора.
Формат входных данных : Сначала вводится кол - во процессов.После этого процессы в формате P T
Формат выходных данных : Кол - во переключений процессора.*/

#include <iostream>
#include <assert.h>

#define START_BUFF_SIZE 4

template <class T, class Comparator>
class Heap
{
public:
    Heap(Comparator _comparator);
    Heap(T* _arr, int arrSize, Comparator _comparator);

    Heap(const Heap& _heap) = delete;
    Heap& operator=(const Heap& _heap) = delete;
    ~Heap();

    void Insert(const T& elem);
    T ExtractTop();
    const T& PeekTop() const { return buffer[0]; }
    int Size() const { return size; }

private:
    Comparator comparator;
    T* buffer;
    int bufferSize;
    int size;

    void buildHeap();
    void shiftDown(int index);
    void shiftUp(int index);
};

template <class T, class Comparator>
Heap<T, Comparator>::Heap(Comparator _comparator) : comparator(_comparator), bufferSize(START_BUFF_SIZE), size(0) {
    buffer = new T[START_BUFF_SIZE];
}

template <class T, class Comparator>
Heap<T, Comparator>::Heap(T* _arr, int arrSize, Comparator _comparator) : comparator(_comparator), bufferSize(arrSize), size(arrSize) {
    buffer = new T[bufferSize];
    for (int i = 0; i < size; ++i)
    {
        buffer[i] = _arr[i];
    }
    buildHeap();
}

template <class T, class Comparator>
Heap<T, Comparator>::~Heap() {
    delete[] buffer;
}

template <class T, class Comparator>
void Heap<T, Comparator>::buildHeap() {
    for (int i = size / 2 - 1; i >= 0; --i)
    {
        shiftDown(i);
    }
}

template <class T, class Comparator>
T Heap<T, Comparator>::ExtractTop() {
    assert(size > 0);
    T result = buffer[0];
    buffer[0] = buffer[size - 1];
    size--;
    if (size > 0)
        shiftDown(0);
    return result;
}

template <class T, class Comparator>
void Heap<T, Comparator>::shiftDown(int index) {
    while ((2 * index + 1) < size) {
        int left_child = 2 * index + 1;
        int right_child = 2 * index + 2;
        int selected_child = left_child;

        if (right_child < size && comparator( buffer[left_child], buffer[right_child] )) {
            selected_child = right_child;
        }
        if (comparator(buffer[index], buffer[selected_child])) {
            break;
        }
        T elem = buffer[index];
        buffer[index] = buffer[selected_child];
        buffer[selected_child] = elem;
        index = selected_child;
    }
}

template <class T, class Comparator>
void Heap<T, Comparator>::shiftUp(int index) {
    while (index > 0)
    {
        int parent = (index - 1) / 2;
        if (comparator(buffer[parent], buffer[index]))
            return;
        T elem = buffer[index];
        buffer[index] = buffer[parent];
        buffer[parent] = elem;
        index = parent;
    }
}

template <class T, class Comparator>
void Heap<T, Comparator>::Insert(const T& elem) {
    if (size >= bufferSize) {
        T* array = new T[bufferSize * 2];
        for (int i = 0; i < size; ++i)
        {
            array[i] = buffer[i];
        }
        bufferSize *= 2;
        delete[] buffer;
        buffer = array;
    }
    buffer[size] = elem;
    ++size;
    shiftUp(size - 1);
}

struct Process {
    int priority;
    int curr_time;
    int target_time;
};


struct ProcessComparator {
    bool operator()(const Process& left, const Process& right) { return (left.priority * (left.curr_time + 1)) < (right.priority * (right.curr_time + 1)); }
};

int switchProcesses(Process* array, int countOfProcesses) {
    int countOfSwitches = 0;
    ProcessComparator processComparator;
    Heap<Process, ProcessComparator> heap(array, countOfProcesses, processComparator);
    while (heap.Size() > 0)
    {
        Process process = heap.ExtractTop();
        ++countOfSwitches;
        process.curr_time += process.priority;
        if (process.curr_time < process.target_time)
            heap.Insert(process);
    }
    return countOfSwitches;
}

int main() {
    int countOfProcesses = 0;
    std::cin >> countOfProcesses;
    Process* processes = new Process[countOfProcesses];
    for (int i = 0; i < countOfProcesses; ++i)
    {
        std::cin >> processes[i].priority >> processes[i].target_time;
        processes[i].curr_time = 0;
    }
    int result = switchProcesses(processes, countOfProcesses);
    delete[] processes;
    std::cout << result;
}

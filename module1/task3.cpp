/*3_1. Реализовать очередь с динамическим зацикленным буфером (на основе динамического массива).

Требования: Очередь должна быть реализована в виде класса.*/

#include <iostream>
#include <cassert>

#define MAX_COMMAND_COUNT 1000000
#define POP_FRONT 2
#define PUSH_BACK 3


template <class T>
class Queue {
public:
    Queue();
    ~Queue();

    Queue ( Queue& ) = delete;
    Queue& operator=( const Queue& ) = delete;

    void Push_back( const T& data );
    T Pop_front();

    size_t Size() const;
    bool IsEmpty() const;

private:
    T* array;
    size_t capacity;
    size_t size;
    size_t front;
    size_t back;


    void rebase();
};

template <class T>
Queue<T>::Queue(): array(nullptr), capacity( 0 ), size( 0 ), front ( 0 ), back ( 0 ) {
    array = new T[capacity];
}

template <class T>
Queue<T>::~Queue() {
    delete[] array;
}

template <class T>
size_t Queue<T>::Size() const{
    return size;
}

template <class T>
bool Queue<T>::IsEmpty() const {
    return size == 0;

}

template <class T>
void Queue<T>::rebase() {
    T* newArray = new T[2 * capacity];
    if (front <= back) {
        for (size_t i = front; i <= back; ++i) {
            newArray[i - front] = array[i];
        }
    } else {
        size_t j = 0;
        for (size_t i = front; i < capacity; ++i) {
            newArray[j++] = array[i];
        }
        for (size_t i = 0; i <= back; ++i) {
            newArray[j++] = array[i];
        }
    }
    front = 0;
    capacity *= 2;
    back = size - 1;
    delete[] array;
    array = newArray;
}

template <class T>
void Queue<T>::Push_back( const T& data ) {

    if (capacity == 0) {
        capacity = 1;
        back = capacity - 1;
        array = new int[capacity];
    }

    if (size == capacity) {
        rebase();
    }

    if (back + 1 == capacity) {
        back = 0;
    } else {
        back++;
    }
    array[back] = data;
    size++;

}

template <class T>
T Queue<T>::Pop_front() {
    T result;
    if (size == 0) {
        result = -1;
    } else {
        size--;
        if (front + 1 == capacity) {
            front = 0;
            result = array[capacity - 1];
        } else {
            result = array[front++];
        }
    }
    return result;
}



void commandControl(Queue<int>& q, const int command, const int elem, bool& res) {
    switch (command) {
        case PUSH_BACK:
            q.Push_back(elem);
            break;
        case POP_FRONT:
            int temp_elem = q.Pop_front();
            if (temp_elem != elem) {
                res = false;
            }
            break;
    }
}


int main() {
    int n = 0; // command count
    std::cin >> n;
    assert(n >= 0 && n <= MAX_COMMAND_COUNT);

    Queue<int> queue;
    int command = 0;
    int element = 0;
    bool result = true;
    for (int i = 0; i < n; ++i) {
        std::cin >> command >> element;
        assert(command == POP_FRONT || command == PUSH_BACK);
        commandControl(queue, command, element, result);
    }

   std::cout << (result ? "YES" : "NO");

    return 0;
}

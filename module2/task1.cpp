/* Задача 1 Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки. */

#include <iostream>
#include <string>
#include <vector>

#define REHASH_THRESHOLD 0.75
#define INITIAL_CAPACITY 8


struct StringHasher {
    size_t operator() (const std::string& str) const {
        size_t hash = 0;
        for (const char& symbol : str) {
            hash = hash * 31 + symbol;
        }
        return hash;
    }
};

template <typename T, typename HashFunc>
class HashTable {
public:
    explicit HashTable(const HashFunc& _hashFunc) : hashFunc(_hashFunc), size(0) {
        storage.resize(INITIAL_CAPACITY);
    }
    HashTable(const HashTable<T, HashFunc>& table) = delete;
    HashTable& operator=(const HashTable& table) = delete;
    ~HashTable() = default;

    bool insert(const T& value);
    bool contains(const T& value);
    bool erase(const T& value);
private:
    struct Node {
        T value;
        size_t hash;
        enum State {
            EMPTY,
            DELETED,
            FILLED
        } state;

        Node() : hash(0), state(EMPTY) {}
        Node(T _value, const size_t& _hash) : value(std::move(_value)), hash(_hash), state(FILLED) {}
        ~Node() = default;
    };

    void rehash();

    HashFunc hashFunc;
    std::vector<Node> storage;
    size_t size;
};

template <typename T, typename HashFunc>
bool HashTable<T, HashFunc>::insert(const T& value) {
    if ((float)size >= REHASH_THRESHOLD * (float)storage.size()) {
        rehash();
    }

    int firstDeletedPos = -1;
    size_t absHash = hashFunc(value);
    size_t hash = absHash % storage.size();
    size_t counter = 0;
    while (storage[hash].state != Node::EMPTY && counter < storage.size()) {
        if (storage[hash].state == Node::DELETED && firstDeletedPos == -1) {
            firstDeletedPos = static_cast<int>(hash);
        }
        if (storage[hash].value == value && storage[hash].state != Node::DELETED) {
            return false;
        }
        hash = (hash + counter + 1) % storage.size();
        ++counter;
    }
    hash = ((firstDeletedPos == -1) ? hash : firstDeletedPos);
    storage[hash] = Node(std::move(value), absHash);
    ++size;
    return true;
}

template <typename T, typename HashFunc>
bool HashTable<T, HashFunc>::contains(const T& value) {
    size_t hash = hashFunc(value) % storage.size();
    size_t counter = 0;
    while (storage[hash].state != Node::EMPTY && counter < storage.size()) {
        if (storage[hash].value == value && storage[hash].state != Node::DELETED) {
            return true;
        }
        hash = (hash + counter + 1) % storage.size();
        ++counter;
    }
    return false;
}

template <typename T, typename HashFunc>
bool HashTable<T, HashFunc>::erase(const T& value) {
    size_t hash = hashFunc(value) % storage.size();
    size_t counter = 0;
    while (storage[hash].state != Node::EMPTY && counter < storage.size()) {
        if (storage[hash].value == value && storage[hash].state != Node::DELETED) {
            storage[hash].state = Node::DELETED;
        	--size;
            return true;
        }
        hash = (hash + counter + 1) % storage.size();
        ++counter;
    }
    return false;
}


template <typename T, typename HashFunc>
void HashTable<T, HashFunc>::rehash() {
    std::vector<Node> newStorage(storage.size() * 2);
    for (const auto& node : storage) {
        if (node.state == Node::FILLED) {
            size_t hash = node.hash % newStorage.size();
            size_t counter = 0;
            while (newStorage[hash].state != Node::EMPTY) {
                hash = (hash + counter + 1) % newStorage.size();
                ++counter;
            }
            newStorage[hash] = node;
        }
    }
    storage = std::move(newStorage);
}

int main() {
    char action;
    std::string value;
    StringHasher hasher;
    HashTable<std::string, StringHasher> hashTable(hasher);
    while (!std::cin.eof()) {
        std::cin >> action >> value;
        switch(action) {
            case '+':
                std::cout << ( hashTable.insert( value ) ? "OK\n" : "FAIL\n" );
                break;
            case '-':
                std::cout << ( hashTable.erase( value ) ? "OK\n" : "FAIL\n" );
                break;
            case '?':
                std::cout << ( hashTable.contains( value ) ? "OK\n" : "FAIL\n" );
                break;
            default:
                break;
        }
        action = ' ';
        value.clear();
    }
    return 0;
}

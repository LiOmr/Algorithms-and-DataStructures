/* 2_1. Обход дерева в порядке in-order */

#include <cassert>
#include <iostream>
#include <stack>

struct DefaultCompare
{
    bool operator()(int left, int right) { return left < right; }
};

template<class T, class Compare = DefaultCompare>
class BinaryTree {
public:
    BinaryTree(const Compare& _cmp = DefaultCompare()) : root(nullptr), cmp(_cmp) {}
    ~BinaryTree();

    BinaryTree(const BinaryTree&) = delete;
    BinaryTree(BinaryTree&&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;
    BinaryTree& operator=(BinaryTree&&) = delete;

    void Insert(const T& key);
    void TraverseInOrder(void (*visitorFunction)(const T& key)) const;

private:
    struct Node {
        Node* ChildLeft;
        Node* ChildRight;
        T KeyValue;
        Node(const T& key) : ChildLeft(nullptr), ChildRight(nullptr), KeyValue(key) {}
    };
    Node* root;
    Compare cmp;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    if (!root){
        return;
    }

    Node *cur = root;
    std::stack<Node *> Stack;
    Stack.push(cur);
    while(!Stack.empty()){
        Node *node = Stack.top();
        Stack.pop();
        if(node->ChildRight){
           Stack.push(node->ChildRight);
        }
        if(node->ChildLeft){
           Stack.push(node->ChildLeft);
        }
        delete node;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Insert(const T& key)
{
    if(root == nullptr) {
        root = new Node(key);
        return;
    }

    Node* node = root;
    Node *parent = nullptr;
    while(node != nullptr) {
        parent = node;
        if(cmp(key, node->KeyValue)) {
            node = node->ChildLeft;
        }
        else {
            node = node->ChildRight;
        }
    }
    node = new Node(key);
    if (parent->KeyValue > key){
        parent->ChildLeft = node;
    } else {
        parent->ChildRight = node;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::TraverseInOrder(void (*visitorFunction)(const T& key)) const {
    if (root == nullptr)
        return;
    
    std::stack<Node *> Stack;
    Node *cur = root;

    while (cur != nullptr || !Stack.empty()) {
        while (cur != nullptr) {
            Stack.push(cur);
            cur = cur->ChildLeft;
        }

        cur = Stack.top();
        Stack.pop();

        visitorFunction(cur->KeyValue);
        cur = cur->ChildRight;
    }
}

int main() {
    size_t size = 0;
    std::cin >> size;
    assert(size > 0);

    BinaryTree<int> tree;
    for (size_t i = 0; i < size; ++i) {
        int num = 0;
        std::cin >> num;
        tree.Insert(num);
    }

    tree.TraverseInOrder([](const int& key) { std::cout << key << " "; });

    return 0;
}

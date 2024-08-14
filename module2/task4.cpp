/* 4_2. Порядковые статистики 
Дано число N и N строк. 
Каждая строка содержащит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики. 
Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”. 
Запрос на получение k-ой порядковой статистики задается числом k. 
Требуемая скорость выполнения запроса - O(log n).
*/


#include <iostream>
#include <cassert>
#include <stack>

template<class T>
class IsLess {
public:
    bool operator()( const T& left, const T& right ) {
        return left < right;
    }
};

template <typename T, typename Order = IsLess<T>>
class BalancedTree {
    struct TreeNode {
        explicit TreeNode(const T &val): val(val), leftChild(nullptr), rightChild(nullptr), treeHeight(1), totalNodes(1) {}
        T val;
        TreeNode *leftChild;
        TreeNode *rightChild;
        size_t treeHeight;
        size_t totalNodes;
    };

 public:
    BalancedTree();

    ~BalancedTree();
    BalancedTree(const BalancedTree& other) = delete;
    BalancedTree& operator=(const BalancedTree& other) = delete;

    void insert(const T &item);
    void erase(const T &item);
    T findStatistic(const size_t &position);
    const T &findKStat(size_t k) const;

 private:
    TreeNode *rootNode;
    Order order;

    size_t getNodeHeight(const TreeNode *node) const;
    int getBalance(const TreeNode *node) const;

    TreeNode *rotateLeft(TreeNode *node);
    TreeNode *rotateRight(TreeNode *node);
    TreeNode *rebalance(TreeNode *node);

    TreeNode *insert(TreeNode *node, const T &item);
    TreeNode *erase(TreeNode *node, const T &item);
    TreeNode *minNodeErase(TreeNode *node, TreeNode *&minNode);

};

template <typename T, typename Order>
BalancedTree<T, Order>::BalancedTree() : rootNode(nullptr), order(Order()) {}

template <typename T, typename Order>
BalancedTree<T, Order>::~BalancedTree() {
    if (!rootNode) {
        return;
    }

    std::stack<TreeNode *> nodeStack;
    nodeStack.push(rootNode);

    while (!nodeStack.empty()) {
        TreeNode *currentNode = nodeStack.top();
        nodeStack.pop();

        if (currentNode->leftChild) {
            nodeStack.push(currentNode->leftChild);
        }

        if (currentNode->rightChild) {
            nodeStack.push(currentNode->rightChild);
        }

        delete currentNode;
    }
}

template <class T, class Order>
size_t BalancedTree<T, Order>::getNodeHeight(const TreeNode *node) const {
    return node ? node->treeHeight : 0;
}

template <class T, class Order>
int BalancedTree<T, Order>::getBalance(const TreeNode *node) const {
    return getNodeHeight(node->rightChild) - getNodeHeight(node->leftChild);
}

template <typename T, typename Order>
typename BalancedTree<T, Order>::TreeNode *
BalancedTree<T, Order>::rotateLeft(TreeNode *node) {
    TreeNode *rightSub = node->rightChild;
    node->rightChild = rightSub->leftChild;
    rightSub->leftChild = node;

    node->treeHeight = std::max(getNodeHeight(node->leftChild), getNodeHeight(node->rightChild)) + 1;
    node->totalNodes = 1 + (node->leftChild ? node->leftChild->totalNodes : 0) + (node->rightChild ? node->rightChild->totalNodes : 0);
    rightSub->treeHeight = std::max(getNodeHeight(rightSub->leftChild), getNodeHeight(rightSub->rightChild)) + 1;
    rightSub->totalNodes = 1 + (rightSub->leftChild ? rightSub->leftChild->totalNodes : 0) + (rightSub->rightChild ? rightSub->rightChild->totalNodes : 0);

    return rightSub;
}

template <typename T, typename Order>
typename BalancedTree<T, Order>::TreeNode *
BalancedTree<T, Order>::rotateRight(TreeNode *node) {
    TreeNode *leftSub = node->leftChild;
    node->leftChild = leftSub->rightChild;
    leftSub->rightChild = node;

    node->treeHeight = std::max(getNodeHeight(node->leftChild), getNodeHeight(node->rightChild)) + 1;
    node->totalNodes = 1 + (node->leftChild ? node->leftChild->totalNodes : 0) + (node->rightChild ? node->rightChild->totalNodes : 0);
    leftSub->treeHeight = std::max(getNodeHeight(leftSub->leftChild), getNodeHeight(leftSub->rightChild)) + 1;
    leftSub->totalNodes = 1 + (leftSub->leftChild ? leftSub->leftChild->totalNodes : 0) + (leftSub->rightChild ? leftSub->rightChild->totalNodes : 0);

    return leftSub;
}

template <typename T, typename Order>
typename BalancedTree<T, Order>::TreeNode *
BalancedTree<T, Order>::rebalance(TreeNode *node) {
    node->treeHeight = std::max(getNodeHeight(node->leftChild), getNodeHeight(node->rightChild)) + 1;
    node->totalNodes = 1 + (node->leftChild ? node->leftChild->totalNodes : 0) + (node->rightChild ? node->rightChild->totalNodes : 0);

    int balanceFactor = getBalance(node);

    if (balanceFactor == 2) {
        if (getBalance(node->rightChild) < 0) {
            node->rightChild = rotateRight(node->rightChild);
        }

        return rotateLeft(node);
    }

    if (balanceFactor == -2) {
        if (getBalance(node->leftChild) > 0) {
            node->leftChild = rotateLeft(node->leftChild);
        }

        return rotateRight(node);
    }

    return node;
}

template <typename T, typename Order>
void BalancedTree<T, Order>::insert(const T &item) {
    rootNode = insert(rootNode, item);
}

template <typename T, typename Order>
typename BalancedTree<T, Order>::TreeNode *
BalancedTree<T, Order>::insert(TreeNode *node, const T &item) {
    if (!node) {
        return new TreeNode(item);
    }

    if (order(item, node->val)) {
        node->leftChild = insert(node->leftChild, item);
    } else {
        node->rightChild = insert(node->rightChild, item);
    }

    return rebalance(node);
}

template <typename T, typename Order>
void BalancedTree<T, Order>::erase(const T &item) {
    rootNode = erase(rootNode, item);
}

template <typename T, typename Order>
typename BalancedTree<T, Order>::TreeNode *
BalancedTree<T, Order>::erase(TreeNode *node, const T &item) {
    if (!node) {
        return nullptr;
    }

    if (order(item, node->val)) {
        node->leftChild = erase(node->leftChild, item);
    } else if (order(node->val, item)) {
        node->rightChild = erase(node->rightChild, item);
    } else {
        TreeNode *leftSub = node->leftChild, *rightSub = node->rightChild;
        delete node;
        if (!rightSub) return leftSub;

        TreeNode *minNode = nullptr;
        TreeNode *newSubTree = minNodeErase(rightSub, minNode);
        minNode->rightChild = newSubTree;
        minNode->leftChild = leftSub;

        return rebalance(minNode);
    }

    return rebalance(node);
}

template <typename T, typename Order>
typename BalancedTree<T, Order>::TreeNode *
BalancedTree<T, Order>::minNodeErase(TreeNode *node, TreeNode *&minNode) {
    if (!node->leftChild) {
        minNode = node;
        return node->rightChild;
    }

    node->leftChild = minNodeErase(node->leftChild, minNode);
    return rebalance(node);
}


template <class T, class Order>
const T &BalancedTree<T, Order>::findKStat(size_t k) const {
    assert(rootNode);
    assert(k < rootNode->totalNodes);

    TreeNode *node = rootNode;
    size_t leftSize = node->leftChild ? node->leftChild->totalNodes : 0;
    while (k != leftSize) {
        if (!node->leftChild) {
            --k;
            node = node->rightChild;
            leftSize = node->leftChild ? node->leftChild->totalNodes : 0;
        } else if (leftSize < k) {
            k -= leftSize + 1;
            node = node->rightChild;
            leftSize = node->leftChild ? node->leftChild->totalNodes : 0;
        } else if (leftSize > k) {
            node = node->leftChild;
            leftSize = node->leftChild ? node->leftChild->totalNodes : 0;
        } else {
            --k;
        }
    }

    return node->val;
}

int main(void) {
    BalancedTree<int> tree;
    size_t totalOperations = 0;
    assert(std::cin >> totalOperations);
    assert(totalOperations > 0);

    size_t queryIndex = 0;
    int currentValue = 0;

    for (size_t i = 0; i < totalOperations; i++) {
        std::cin >> currentValue >> queryIndex;

        if (currentValue < 0) {
            tree.erase(-currentValue);
        } else {
            tree.insert(currentValue);
        }

        std::cout << tree.findKStat(queryIndex) << "\n";
    }

    std::cout << std::endl;
    return 0;
}


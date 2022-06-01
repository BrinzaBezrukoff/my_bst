#ifndef MY_BST_BINARY_SEARCH_TREE_H
#define MY_BST_BINARY_SEARCH_TREE_H

#include <utility>
#include <cstddef>
#include <deque>
#include <stack>

/*!
    Имплементация бинарного дерева поиска
    Допускается дублирование ключей (аналог multimap)
*/
template <typename Key, typename Value>
class BinarySearchTree
{
    struct Node
    {
        Node(Key key,
             Value value,
             Node* parent = nullptr,
             Node* left = nullptr,
             Node* right = nullptr):
                 keyValuePair(std::make_pair(key, value)),
                 parent(parent),
                 left(left),
                 right(right) {

        }

        const Key& key() const {
            return keyValuePair.first;
        }

        const Value& value() const {
            return keyValuePair.second;
        }

        std::pair<Key, Value> keyValuePair;
        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
    };

public:
    //! Конструктор по умолчанию
    BinarySearchTree() = default;

    //! Копирование
    BinarySearchTree(const BinarySearchTree& other) {
        std::deque<std::pair<Node*, Node*>> queue;
        _root = new Node(other._root->key(), other._root->value());
        queue.push_back(std::make_pair(other._root, _root));
        while (!queue.empty()) {
            auto [origin, copy] = queue.front();
            if (origin == other._leftNode) {
                _leftNode = copy;
            }
            if (origin == other._rightNode) {
                _rightNode = copy;
            }
            queue.pop_front();
            if (origin->left != nullptr) {
                copy->left = new Node(origin->left->key(), origin->left->value(), copy);
                queue.push_back(std::make_pair(origin->left, copy->left));
            }
            if (origin->right != nullptr) {
                copy->right = new Node(origin->right->key(), origin->right->value(), copy);
                queue.push_back(std::make_pair(origin->right, copy->right));
            }
        }
        _size = other._size;
    }

    BinarySearchTree& operator=(const BinarySearchTree other) {
        swap(other, *this);
        return *this;
    }

    //! Перемещение
    BinarySearchTree(BinarySearchTree&& other) noexcept {
        swap(*this, other);
    }

//    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {  // TODO: need or not? copy-swap idiom
//        return *this;
//    }

    //! Деструктор
    ~BinarySearchTree() {
        clear();
    }

    //! Обмен
    friend void swap(BinarySearchTree& first, BinarySearchTree& second) noexcept {
        using std::swap;
        std::swap(first._root, second._root);
        std::swap(first._leftNode, second._leftNode);
        std::swap(first._rightNode, second._rightNode);
        std::swap(first._size, second._size);
    }

    /*!
        Константный итератор
    */
    class ConstIterator
    {
    public:
        explicit ConstIterator(const Node* node):
                _node(node)
        {

        }

        const std::pair<Key, Value>& operator*() const {
            return _node->keyValuePair;
        }

        const std::pair<Key, Value>* operator->() const {
            return &_node->keyValuePair;
        }

        ConstIterator operator++() {
            if (_node->right == nullptr) {
                while (_node->parent != nullptr && _node->parent->right == _node) {
                    _node = _node->parent;
                }
                _node = _node->parent;
            }
            else {
                _node = _node->right;
                while (_node->left != nullptr) {
                    _node = _node->left;
                }
            }
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator temp (*this);
            ++*this;
            return temp;
        }

        ConstIterator operator--() {
            if (_node->left == nullptr) {
                while (_node->parent != nullptr && _node->parent->left == _node) {
                    _node = _node->parent;
                }
                _node = _node->parent;
            } else {
                _node = _node->left;
                while (_node->right != nullptr) {
                    _node = _node->right;
                }
            }
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator temp(*this);
            ++*this;
            return temp;
        }

        bool operator==(const ConstIterator& other) const {
            return other._node == _node;
        }

        bool operator!=(const ConstIterator& other) const {
            return other._node != _node;
        }

    private:
        const Node* _node;
    };

    /*!
        Итератор бинарного дерева поиска

        Обходит дерево последовательно от меньшего ключа к большему
    */
    class Iterator
    {
    public:
        explicit Iterator(Node* node):
            _node(node)
        {

        }

        std::pair<Key, Value>& operator*() {
            return _node->keyValuePair;
        }

        const std::pair<Key, Value>& operator*() const {
            return _node->keyValuePair;
        }

        std::pair<Key, Value>* operator->() {
            return &_node->keyValuePair;
        }

        const std::pair<Key, Value>* operator->() const {
            return &_node->keyValuePair;
        }

        Iterator operator++() {
            if (_node->right == nullptr) {
                while (_node->parent != nullptr && _node->parent->right == _node) {
                    _node = _node->parent;
                }
                _node = _node->parent;
            }
            else {
                _node = _node->right;
                while (_node->left != nullptr) {
                    _node = _node->left;
                }
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp (*this);
            ++*this;
            return temp;
        }

        Iterator operator--() {
            if (_node->left == nullptr) {
                while (_node->parent != nullptr && _node->parent->left == _node) {
                    _node = _node->parent;
                }
                _node = _node->parent;
            } else {
                _node = _node->left;
                while (_node->right != nullptr) {
                    _node = _node->right;
                }
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp (*this);
            ++*this;
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return other._node == _node;
        }

        bool operator!=(const Iterator& other) const {
            return other._node != _node;
        }

        operator ConstIterator() {
            return ConstIterator(_node);
        }

    private:
        Node* _node;
    };

    // вставить элемент с ключем key и значением value
    void insert(const Key& key, const Value& value) {
        if (_root == nullptr) {
            _root = _leftNode = _rightNode = new Node(key, value);
            _size++;
            return;
        }
        Node *newNode;
        Node *node = _root;
        while (1) {
            if (key <= node->key()) {
                if (node->left == nullptr) {
                    node->left = newNode = new Node(key, value, node);
                    break;
                } else {
                    node = node->left;
                }
            } else {
                if (node->right == nullptr) {
                    node->right = newNode = new Node(key, value, node);
                    break;
                } else {
                    node = node->right;
                }
            }
        }
        if (newNode->parent == _leftNode && newNode->parent->left == newNode) {
            _leftNode = newNode;
        }
        else if (newNode->parent == _rightNode && newNode->parent->right == newNode) {
            _rightNode = newNode;
        }
        _size++;
    }

    // удалить все элементы с ключем key
    void erase(const Key& key) {
        Node *node;
        while ((node = findNode(key)) != nullptr) {
            eraseNode(node);
        }
    }

    // очистить дерево
    void clear() {
        std::deque<Node*> queue;
        queue.push_back(_root);
        Node* node;
        while (!queue.empty()) {
            node = queue.front();
            queue.pop_front();
            if (node == nullptr) {
                continue;
            }
            if (node->left != nullptr) {
                queue.push_back(node->left);
            }
            if (node->right != nullptr) {
                queue.push_back(node->right);
            }
            delete node;
        }
        _root = _leftNode = _rightNode = nullptr;
        _size = 0;
    }

    // найти первый элемент в дереве, равный ключу key
    ConstIterator find(const Key& key) const {
        return ConstIterator(findNode(key));
    }

    Iterator find(const Key& key) {
        return Iterator(findNode(key));
    }

    // найти все элементы, у которых ключ равен key
    // первый итератор пары - первый элемент в дереве, равный key
    // второй итератор пары - первый элемент в дереве больший, чем key
    // [pair.first, pair.second) - полуинтервал, содержащий все элементы с ключем key
    std::pair<Iterator, Iterator> equalRange(const Key& key) {
        Iterator first = find(key);
        if (first == end()) {
            return std::make_pair(first, first);
        }
        Iterator second (first);
        second++;
        while (first != begin()) {
            --first;
            if (first->first != key) {
                first++;
                break;
            }
        }
        return std::make_pair(first, second);
    }

    std::pair<ConstIterator, ConstIterator> equalRange(const Key& key) const {
        ConstIterator first = find(key);
        if (first == cend()) {
            return std::make_pair(first, first);
        }
        ConstIterator second (first);
        second++;
        while (first != cbegin()) {
            --first;
            if (first->first != key) {
                first++;
                break;
            }
        }
        return std::make_pair(first, second);
    }

    // получить итератор на минимальное значение в дереве
    ConstIterator min(const Key& key) const {
        auto [first, second] = equalRange(key);
        ConstIterator itMin (first);
        while (++first != second) {
            if (first->second < itMin->second) {
                itMin = first;
            }
        }
        return itMin;
    }

    // получить итератор на максимальное значение в дереве
    ConstIterator max(const Key& key) const {
        auto [first, second] = equalRange(key);
        ConstIterator itMax (first);
        while (++first != second) {
            if (first->second > itMax->second) {
                itMax = first;
            }
        }
        return itMax;
    }

    Iterator begin() {
        return Iterator(_leftNode);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    ConstIterator cbegin() const {
        return ConstIterator(_leftNode);
    }

    ConstIterator cend() const {
        return ConstIterator(nullptr);
    }

    size_t size() const {
        return _size;
    }

    friend std::ostream& operator<<(std::ostream& stream, const BinarySearchTree& bst) {
        std::deque<Node*> queue;
        Node* node;
        queue.push_back(bst._root);
        size_t rowCount = 1;
        bool running = true;
        while (running) {
            running = false;
            for (size_t i = 0; i < rowCount; i++) {
                node = queue.front();
                queue.pop_front();
                if (node != nullptr) {
                    running = true;
                    stream << node->key() << ":" << node->value() << " ";
                    queue.push_back(node->left);
                    queue.push_back(node->right);
                }
                else {
                    queue.push_back(nullptr);
                    queue.push_back(nullptr);
                    stream << "NULL ";
                }
            }
            stream << std::endl;
            rowCount *= 2;
        }
        return stream;
    }

private:
    size_t _size = 0;
    Node* _root = nullptr; //!< корневой узел дерева
    Node* _leftNode = nullptr;
    Node* _rightNode = nullptr;

    const Node* findNode(const Key& key) const {
        const Node* node = _root;
        while (node != nullptr) {
            if (key < node->key()) {
                node = node->left;
            }
            else if (key > node->key()) {
                node = node->right;
            }
            else {
                break;
            }
        }
        return node;
    }

    Node* findNode(const Key& key) {
        return const_cast<Node*>(const_cast<const BinarySearchTree*>(this)->findNode(key));
    }

    void eraseNode(Node* node) {
        if (node->left == nullptr && node->right == nullptr) {  // no child nodes
            if (node->parent->left == node) {
                node->parent->left = nullptr;
            }
            else {
                node->parent->right = nullptr;
            }
            delete node;
        }
        else if (node->left == nullptr || node->right == nullptr) {  // one child node
            Node* child = (node->left != nullptr) ? node->left : node->right;
            node->keyValuePair = child->keyValuePair;
            node->left = child->left;
            node->right = child->right;
            if (child->right != nullptr) {
                child->right->parent = node;
            }
            if (child->left != nullptr) {
                child->left->parent = node;
            }
            delete child;
        }
        else {  // two child nodes
            if (node->right->left == nullptr) {  // right node can simply replace erased node
                node->keyValuePair = node->right->keyValuePair;
                Node* newRight = node->right->right;
                delete node->right;
                node->right = newRight;
                if (newRight != nullptr) {
                    newRight->parent = node;
                }
            }
            else {  // erased node can be replaced by the most left node in right subtree
                Node* left = node->right->left;
                while (left->left != nullptr) {
                    left = left->left;
                }
                node->keyValuePair = left->keyValuePair;
                left->parent->left = left->right;
                delete left;
            }
        }
    }
};

//
///*!
//    Имплементация словаря
//    Не допускается дублирование ключей (аналог std::map)
//*/
//template <typename Key, typename Value>
//class Map
//{
//    BinarySearchTree<Key, Value> _tree;
//public:
//    using MapIterator = typename BinarySearchTree<Key, Value>::Iterator;
//    using ConstMapIterator = typename BinarySearchTree<Key, Value>::ConstIterator;
//
//    Map() = default;
//
//    explicit Map(const Map& other);
//    Map& operator=(const Map& other);
//
//    explicit Map(Map&& other) noexcept;
//    Map& operator=(Map&& other) noexcept;
//
//    ~Map() = default;
//
//    // вставить элемент с ключем key и значением value
//    // если узел с ключем key уже представлен, то заменить его значение на value
//    void insert(const Key& key, const Value& value);
//
//    // удалить элемент с ключем key
//    void erase(const Key& key);
//
//    // найти элемент, равный ключу key
//    ConstMapIterator find(const Key& key) const;
//    MapIterator find(const Key& key);
//
//    // доступ к элементу по ключу
//    // если в момент обращения элемента не существует, создать его,
//    // ключ равен key, value равно дефолтному значению для типа Value
//    const Value& operator[](const Key& key) const;
//    Value& operator[](const Key& key);
//
//    MapIterator begin();
//    MapIterator end();
//
//    ConstMapIterator cbegin() const;
//    ConstMapIterator cend() const;
//
//    size_t size() const;
//};
//
//
///*!
//    Имплементация множества
//    Не допускается дублирование ключей (аналог std::set)
//*/
//template <typename Value>
//class Set
//{
//    Map<Value, Value> _map;
//
//public:
//    using SetIterator = Map::MapIterator;
//    using ConstSetIterator = Map::ConstMapIterator;
//
//    Set() = default;
//
//    explicit Set(const Set& other);
//    Set& operator=(const Set& other);
//
//    explicit Set(Set&& other) noexcept;
//    Set& operator=(Set&& other) noexcept;
//
//    ~Set() = default;
//
//    void insert(const Value& value);
//
//    void erase(const Value& value);
//
//    ConstSetIterator find(const Value& value) const;
//    SetIterator find(const Value& key);
//
//    bool contains(const Value& value) const;
//};
//

#endif // MY_BST_BINARY_SEARCH_TREE_H

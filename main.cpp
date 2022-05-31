#include <iostream>
#include "BinarySearchTree.h"


int main() {

    BinarySearchTree<int, std::string> bst;

    bst.insert(20, "kek");
    bst.insert(20, "lol");
    bst.insert(20, "abc");
    bst.insert(10, "efg");
    bst.insert(20, "xyz");

    std::cout << bst;

    auto [start, stop] = bst.equalRange(20);
    for (; start != stop; ++start) {
        std::cout << start->first << " " << start->second << ", ";
    }

    return 0;
}

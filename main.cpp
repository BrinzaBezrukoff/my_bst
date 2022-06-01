#include <iostream>
#include "BinarySearchTree.h"


int main() {

    BinarySearchTree<int, std::string> bst;

    bst.insert(20, "kek");
    bst.insert(20, "lol");
    bst.insert(20, "abc");
    bst.insert(30, "aaa");
    bst.insert(10, "efg");
    bst.insert(20, "xyz");

    std::cout << bst;

    std::cout << "Erasing by key=20" << std::endl;
    bst.erase(20);

    std::cout << bst;

    return 0;
}

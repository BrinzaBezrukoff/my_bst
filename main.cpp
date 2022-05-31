#include <iostream>
#include "BinarySearchTree.h"


int main() {

    BinarySearchTree<int, int> bst;

    bst.insert(10, 2);
    bst.insert(5, 9);
    bst.insert(4, 1);
    bst.insert(7, 8);
    bst.insert(17, 4);
    bst.insert(31, 0);
    bst.insert(6, 5);
    bst.insert(8, 3);

    for (auto&& el: bst) {
        std::cout << el.first << " : " << el.second << std::endl;
    }
    std::cout << std::endl;

    BinarySearchTree<int, int> newbst (std::move(bst));
    newbst.erase(5);

//    for (auto&& el: bst) {
//        std::cout << el.first << " : " << el.second << std::endl;
//    }
//    std::cout << std::endl;

    for (auto&& el: newbst) {
        std::cout << el.first << " : " << el.second << std::endl;
    }
    std::cout << std::endl;

    // TODO: here happens SIGSEGV

    return 0;
}

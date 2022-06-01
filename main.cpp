#include <iostream>
#include "BinarySearchTree.h"

int main() {

    Map<char, unsigned int> dict;
    Set<char> set;
    std::string input ("Hello my dear friend");

    for (auto chr: input) {
        dict[chr]++;
        set.insert(chr);
    }

    for (auto it: dict) {
        std::cout << "'" << it.first << "'" << " : " << it.second << std::endl;
    }

    std::cout << set.contains('r') << std::endl;

    return 0;
}

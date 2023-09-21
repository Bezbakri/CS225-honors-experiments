#include <vector>
#include "BigClass.hpp"

int main() {
    std::vector<BigClass> bc;
    for (int loops = 0; loops < 100; ++loops) {
        for (int i = 0; i < 3; ++i) {
            BigClass newBC;
            bc.push_back(newBC);
        }
    }
    std::cout << "Done" << std::endl;
    return 0;
}
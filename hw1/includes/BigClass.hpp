#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>

class BigClass {
    public:
        BigClass();
        std::string getData();
        unsigned long getNum();
    private:
        std::string data_;
        unsigned long num_;
        std::list<unsigned> ints_;
};

BigClass::BigClass() {
    std::ifstream f;
    f.open("words.txt");
    if (f.is_open()) {
        while (f.good()) {
            std::string stuff;
            f >> stuff;
            data_ += stuff;
        }
    }
    num_ = UINT64_MAX;
    for (unsigned i = 0; i < UINT16_MAX; ++i) {
        ints_.push_back(i);
    }
}

std::string BigClass::getData() {
    return data_;
}

unsigned long BigClass::getNum() {
    return num_;
}
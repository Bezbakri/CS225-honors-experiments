#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <random>

void bubbleSort(std::vector<int>& vec) {
    bool swapped = true;
    size_t n = vec.size();
    do {
        swapped = false;
        for (int i = 1; i < n; ++i) {
            if (vec.at(i-1) > vec.at(i)) {
                std::swap(vec.at(i-1), vec.at(i));
                swapped = true;
            }
        }
        n -= 1;
    } while (swapped);
}



int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "random: must specify number" << std::endl;
        return (404);
    }
    int number = atoi(argv[1]);
    std::vector<int> vec;
    for (int i = 0; i < number; ++i) {
        vec.push_back(i);
    }
    auto start{std::chrono::steady_clock::now()};
    auto end{std::chrono::steady_clock::now()};
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, number - 2); // define the range
    int index = distr(gen);
    std::swap(vec.at(index), vec.at(index + 1));
    std::vector<int> vec2 = vec;

    // Using std::sort
    start = std::chrono::steady_clock::now();
    std::sort(vec.begin(), vec.end());
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds_sort{end - start};
    std::cout << "Sort time for std::sort = " << elapsed_seconds_sort.count() << std::endl;

    // Using bubble sort
    start = std::chrono::steady_clock::now();
    bubbleSort(vec2);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds_sort2{end - start};
    std::cout << "Sort time for bubblesort = " << elapsed_seconds_sort2.count() << std::endl;
    return 0;
}
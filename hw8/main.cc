#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>
#include <chrono>

// int lisRecHelper(const std::vector<int>& vec, int curr) {
//     if (curr == 0) {
//         return 1;
//     }
//     int ans = 1;
//     for (int i = curr; i >= 0; --i) {
//         if (vec.at(i) < vec.at(curr)) {
//             ans = std::max(ans, 1 + lisRecHelper(vec, i));
//         }
//     }
//     return ans;
// }

// int lisRec(const std::vector<int>& vec) {
//     int ans = 1;
//     for (int i = 0; i < vec.size(); ++i) {
//         ans = std::max(ans, lisRecHelper(vec, i));
//     }
//     return ans;
// }

int lisRecHelper(const std::vector<int>& vec, int i, int j) {
    if (j >= vec.size()) {
        return 0;
    } else if (vec.at(i) >= vec.at(j)) {
        return lisRecHelper(vec, i, j+1);
    } else {
        int skip = lisRecHelper(vec, i, j+1);
        int take = lisRecHelper(vec, j, j + 1) + 1;
        return std::max(skip, take);
    }
}

int lisRec(const std::vector<int>& vec) {
    std::vector<int> newVec = vec;
    newVec.insert(newVec.begin(), INT32_MIN);
    return lisRecHelper(newVec, 0, 1);
}

int lisDP(const std::vector<int>& vec) {
    std::vector<int> newVec = vec;
    newVec.insert(newVec.begin(), INT32_MIN);
    std::vector<std::vector<int>> table(newVec.size(), std::vector<int>(newVec.size() + 1));
    for (int i = 0; i < vec.size(); ++i) {
        table.at(i).at(newVec.size()) = 0;
    }
    for (int j = vec.size(); j >= 0; --j) {
        for (int i = 0; i <= j; ++i) {
            int keep = 1 +  table.at(j).at(j+1);
            int skip = table.at(i).at(j + 1);
            if (newVec.at(i) >= newVec.at(j)) {
                table.at(i).at(j) = skip;
            } else {
                table.at(i).at(j) = std::max(keep, skip);
            }
        }
    }
    return table.at(0).at(1);
}


int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "random: must specify number" << std::endl;
        return (404);
    }
    int number = atoi(argv[1]);
    std::vector<int> vec;
    for (int i = 0; i < number; ++i) {
        int num = rand() % number;
        vec.push_back(num);
    }
    //std::vector<int> vec({6, 1, 3, 2, 5, 4, 7, 8, 0, 9});
    auto start{std::chrono::steady_clock::now()};
    auto end{std::chrono::steady_clock::now()};
    start = std::chrono::steady_clock::now();
    int recAns = lisRec(vec);
    end =  std::chrono::steady_clock::now();
    std::cout << "Max increasing subsequence (recursive): " << recAns << std::endl;
    std::chrono::duration<double> elapsed_seconds_rec{end - start};
    std::cout << "time for recursive algorithm = " << elapsed_seconds_rec.count() << std::endl;
    start = std::chrono::steady_clock::now();
    int dpAns = lisDP(vec);
    end =  std::chrono::steady_clock::now();
    std::cout << "Max increasing subsequence (dp) " << dpAns << std::endl;
    std::chrono::duration<double> elapsed_seconds_dp{end - start};
    std::cout << "time for DP algorithm = " << elapsed_seconds_dp.count() << std::endl;
    return 0;
}
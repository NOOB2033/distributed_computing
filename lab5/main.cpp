#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>


long long scalar_mul(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    long long result = 0;

#pragma omp parallel for shared(vec1, vec2) reduction(+ : result)
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }

    return result;
}


int main() {
    std::srand(std::time(nullptr));

    size_t size;
    std::cout << "Enter the size of the vectors:";
    std::cin >> size;

    std::vector<int> vec1(size);
    std::vector<int> vec2(size);

    for (size_t i = 0; i < size; ++i) {
        vec1[i] = rand() % 10;
        vec2[i] = rand() % 10;
    }

    for (size_t i = 0; i < size; ++i) {
        std::cout << vec1[i] << ' ';
    }
    std::cout << std::endl;
    for (size_t i = 0; i < size; ++i) {
        std::cout << vec2[i] << ' ';
    }
    std::cout << std::endl;

    std::cout << scalar_mul(vec1, vec2) << std::endl;
}
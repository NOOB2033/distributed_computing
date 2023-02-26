#include <iostream>
#include <vector>
#include <thread>


void function(std::vector<int>& vec, std::vector<std::thread>& pool, size_t n, size_t k) {
    std::vector<size_t> check(n, 0);
    size_t integer = n / k;
    size_t remainder = n % k;

    for (size_t i = 0; i < k; ++i) {
        pool.emplace_back([&, threadNumber = i]() {
            for (size_t i = integer * threadNumber; i < integer * (threadNumber + 1); ++i) {
                vec[i] = rand() % 100;
                ++check[i];
            }

            if (remainder != 0) {
                size_t index = integer * k + threadNumber;
                if (index < vec.size()) {
                    vec[index] = rand() % 100;
                    ++check[index];
                }
            }
        });
    }

    for (auto&& thread : pool) {
        thread.join();
    }

    for (auto&& val : check) {
        if (val == 0) {
            throw std::runtime_error("Error: element not generated");
        }
        if (val > 1) {
            throw std::runtime_error("Error: element generated more than once");
        }
    }
}


int main() {
    std::srand(std::time(nullptr));
    size_t n, k;

    std::cout << "Array size:";
    std::cin >> n;
    std::cout << "Number of thread:";
    std::cin >> k;
    if (k == 0) {
        std::cout << "Error: 0 threads\n";
        return -1;
    }

    std::vector<int> vec(n);
    std::vector<std::thread> pool;

    try {
        function(vec, pool, n, k);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
        return -2;
    }

    for (auto&& val : vec) {
        std::cout << val << ' ';
    }
}

// 1 1 1 1 1 1 1 1

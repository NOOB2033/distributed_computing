#ifndef DISTRIBUTED_COMPUTING_EXAMPLE_HPP
#define DISTRIBUTED_COMPUTING_EXAMPLE_HPP
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>


class spinlock {
public:
    void lock() {
        while (atom.exchange(true) == true) {
            while (atom.load() == true);
        }
    }

    void unlock() {
        atom.store(false);
    }
private:
    std::atomic<bool> atom{false};
};


void example() {
    while (true) {
        std::vector<std::thread> vec;
        size_t counter = 0;
        spinlock lock;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 10; ++i) {
            vec.emplace_back([&]() {
                for (size_t j = 0; j < 100500; ++j) {
                    lock.lock();
                    counter++;
                    lock.unlock();
                }
            });
        }

        for (auto&& elem : vec) {
            elem.join();
        }
        std::cout << counter << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> d = end - start;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << " milliseconds" << std::endl;
    }
}


#endif //DISTRIBUTED_COMPUTING_EXAMPLE_HPP

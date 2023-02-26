#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>


class CyclicBarrier {
public:

    CyclicBarrier(size_t participants)
        : participants(participants)
    {}

    void arrive() {
        std::unique_lock lock(mutex);

        bool current = state;
        ++counters[current];

        condvar.wait(lock, [&]() {
            return counters[current] == participants;
        });

        if (state == current) {
            state = !state;
            counters[state] = 0;
            condvar.notify_all();
            std::cout << "Done\n";
        }
    }

private:

    bool state{false};
    size_t counters[2]{0, 0};
    const size_t participants;
    std::mutex mutex;
    std::condition_variable condvar;
};


int main() {
    size_t k;
    std::cout << "Number of waiting thread:";
    std::cin >> k;
    if (k == 0) {
        std::cout << "Error: 0 threads\n";
        return -1;
    }

    CyclicBarrier barrier(k);
    std::vector<std::thread> pool;

    for (size_t i = 0; i < k; ++i) {
        pool.emplace_back([&]() {
            barrier.arrive();
        });
    }

    for (auto&& thread : pool) {
        thread.join();
    }
}
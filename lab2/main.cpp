#include <iostream>
#include <condition_variable>
#include <queue>


class BlockingQueue {
public:

    void push(size_t value) {
        std::lock_guard lock(mutex);
        queue.push(std::move(value));
        cv.notify_one();
    }

    size_t pop() {
        std::unique_lock lock(mutex);
        cv.wait(lock, [&]() {
            return !queue.empty();
        });
        auto value = std::move(queue.front());
        queue.pop();
        return value;
    }

private:

    std::queue<size_t> queue;
    std::condition_variable cv;
    std::mutex mutex;
};


int main() {
    BlockingQueue que;
    std::thread t1([&]() {
        for (size_t i = 0; i < 1000; ++i) {
            que.push(i);
        }
    });
    std::thread t2([&]() {
        for (size_t i = 0; i < 1000; ++i) {
            que.pop();
        }
    });
    t1.detach();
    t2.detach();
}
#ifndef DISTRIBUTED_COMPUTING_WAITGROUP_HPP
#define DISTRIBUTED_COMPUTING_WAITGROUP_HPP


#include <mutex>
#include <condition_variable>


namespace tp::detail {

    class WaitGroup {
    public:

        void wait() {
            std::unique_lock guard(mutex);
            while (counter > 0) {
                cv.wait(guard);
            }
        }

        void inc() {
            std::lock_guard guard(mutex);
            ++counter;
        }

        void dec() {
            std::lock_guard guard(mutex);
            if (--counter == 0) {
                cv.notify_all();
            }
        }

    private:

        size_t counter{0};
        std::mutex mutex;
        std::condition_variable cv;
    };
}


#endif //DISTRIBUTED_COMPUTING_WAITGROUP_HPP

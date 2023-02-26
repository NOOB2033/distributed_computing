#ifndef DISTRIBUTED_COMPUTING_BLOCKING_QUEUE_HPP
#define DISTRIBUTED_COMPUTING_BLOCKING_QUEUE_HPP


#include <mutex>
#include <condition_variable>
#include <optional>
#include <deque>


namespace tp {

    template <typename T>
    class UnboundedBlockingQueue {
    public:
        bool Put(T value) {
            std::lock_guard guard(mutex);
            if (close) {
                return false;
            }
            deq.emplace_back(std::move(value));
            cv.notify_one();
            return true;
        }

        std::optional<T> Take() {
            std::unique_lock guard(mutex);
            while (!close && deq.empty()) {
                cv.wait(guard);
            }
            if (deq.empty()) {
                return std::nullopt;
            }
            auto value(std::move(deq.front()));
            deq.pop_front();
            return value;
        }

        void Close() {
            std::lock_guard guard(mutex);
            CloseImpl(false);
        }

        void Cancel() {
            std::lock_guard guard(mutex);
            CloseImpl(true);
        }

    private:
        void CloseImpl(bool clear) {
            close = true;
            if (clear) {
                deq.clear();
            }
            cv.notify_all();
        }

    private:

        bool close{false};
        std::deque<T> deq;
        std::mutex mutex;
        std::condition_variable cv;
    };

}  // namespace tp


#endif //DISTRIBUTED_COMPUTING_BLOCKING_QUEUE_HPP

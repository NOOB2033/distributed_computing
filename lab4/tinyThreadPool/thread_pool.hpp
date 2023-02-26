#ifndef DISTRIBUTED_COMPUTING_THREAD_POOL_HPP
#define DISTRIBUTED_COMPUTING_THREAD_POOL_HPP


#include "blocking_queue.hpp"
#include "task.hpp"
#include "WaitGroup.hpp"
#include <vector>
#include <thread>


namespace tp {

    class ThreadPool {
    public:

        explicit ThreadPool(size_t workers);
        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;

        void Submit(Task task);

        void WaitIdle();

        void Stop();

    private:

        void StartWorkerThreads(size_t);
        void WorkerRoutine();

        std::vector<std::thread> vec;
        detail::WaitGroup group;
        UnboundedBlockingQueue<Task> queue;
    };

}  // namespace tp


#endif // DISTRIBUTED_COMPUTING_THREAD_POOL_HPP

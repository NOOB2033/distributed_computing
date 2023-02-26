#include "thread_pool.hpp"
#include <cassert>


namespace tp {


    ThreadPool::ThreadPool(size_t workers) {
        StartWorkerThreads(workers);
    }

    ThreadPool::~ThreadPool() {
        assert(vec.empty());
    }

    void ThreadPool::Submit(Task task) {
        group.inc();
        queue.Put(std::move(task));
    }

    void ThreadPool::WaitIdle() {
        group.wait();
    }

    void ThreadPool::Stop() {
        queue.Cancel();
        for (auto&& thread : vec) {
            thread.join();
        }
        vec.clear();
    }

    void ThreadPool::StartWorkerThreads(size_t workers) {
        for (size_t i = 0; i < workers; ++i) {
            vec.emplace_back([this]() {
                WorkerRoutine();
            });
        }
    }

    void ThreadPool::WorkerRoutine() {
        while (true) {
            auto task = queue.Take();

            if (task.has_value()) {
                try {
                    task.value()();
                } catch (...) {
                }

                group.dec();
            } else {
                break;
            }
        }
    }

}  // namespace tp
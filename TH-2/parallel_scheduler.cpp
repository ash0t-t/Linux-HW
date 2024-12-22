#include "parallel_scheduler.hpp"
#include <stdexcept>

parallel_scheduler::parallel_scheduler(size_t capacity) : capacity(capacity), stop(false) {
    if (capacity == 0) {
        throw std::invalid_argument("thread ppol must be more than 0");
    }

    for (size_t i = 0; i < capacity; ++i) {
        threads.emplace_back(&parallel_scheduler::worker_thread, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        stop = true;
    }
    condition.notify_all();

    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void parallel_scheduler::worker_thread() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this]() { return stop || !tasks.empty(); });

            if (stop && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}
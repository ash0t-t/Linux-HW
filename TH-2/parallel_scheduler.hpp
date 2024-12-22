#ifndef PARALLEL_SCHEDULER_HPP
#define PARALLEL_SCHEDULER_HPP

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>
#include <type_traits>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    template <typename Func, typename Arg>
    auto run(Func&& func, Arg&& arg) -> std::future<std::invoke_result_t<Func, Arg>> {
        using ReturnType = std::invoke_result_t<Func, Arg>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            [func = std::forward<Func>(func), arg = std::forward<Arg>(arg)]() mutable {
                return func(arg);
            });

        std::future<ReturnType> result = task->get_future();

        {
            std::lock_guard<std::mutex> lock(mutex);
            tasks.emplace([task]() { (*task)(); });
        }

        condition.notify_one();
        return result;
    }

private:
    void worker_thread();

    size_t capacity;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mutex;
    std::condition_variable condition;
    bool stop;
};

#endif
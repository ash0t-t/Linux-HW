#include "parallel_scheduler.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int example_function(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return x * x;
}

int main() {
    const size_t capacity = 4;
    const size_t num_tasks = 10;

    parallel_scheduler scheduler(capacity);

    std::vector<std::future<int>> results;

    for (int i = 0; i < num_tasks; ++i) {
        results.push_back(scheduler.run(example_function, i));
    }

    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "result: " << i << ": " << results[i].get() << std::endl;
    }

    return 0;
}
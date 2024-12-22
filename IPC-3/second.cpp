#include "shared_array.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    shared_array array("shared_array", 10);

    while (true) {
        array.lock();
        for (size_t i = 0; i < 10; ++i) {
            array[i] -= 1;
            std::cout << "process 2: array[" << i << "] = " << array[i] << std::endl;
        }
        array.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

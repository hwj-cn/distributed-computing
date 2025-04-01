#include "ThreadPool.h"
#include <iostream>

int main() {
    ThreadPool pool(4);

    for (int i = 0; i < 10; ++i) {
        auto info = pool.submit([i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << std::format("Task {} finished\n", i);
        });

        std::cout << std::format("Submitted task {}\n", info.task_id);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 0;
}
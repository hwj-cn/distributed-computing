#include "ThreadPool.h"
#include <iostream>
#include <random>

int main() {
    ThreadPool pool(4);

    for (int i = 0; i < 10; ++i) {
        auto future = pool.submit([&]() -> int {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << std::format("Task {} finished\n", i);
            // return rand();

            std::random_device rd;
            std::mt19937 mt(rd());

            return mt();
        });

        // std::cout << std::format("Submitted task {}\n", info.task_id);

        std::cout << std::format("Task {}, res = {}\n", i, future.get());
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Stop ThreadPool" << std::endl;

    return 0;
}
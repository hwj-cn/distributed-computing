#include "ThreadPool.h"
#include <iostream>
#include <random>

int main() {
    auto test_submit = []() {
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
    };


    auto test_submit_with_info = []() {
        ThreadPool pool(4);

        std::vector<std::shared_ptr<TaskInfo>> task_infos;
        std::vector<std::future<int>> futures;

        for (int i = 0; i < 10; ++i) {
            auto [info, future] = pool.submit_with_info([i]() -> int {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::cout << std::format("Task {} finished\n", i);
                return i * i;
            });

            task_infos.push_back(info);
            futures.push_back(std::move(future));

            std::cout << std::format("Submitted task {}\n", info->task_id);
        }


        // 任务已经全部提交

        for (size_t i = 0; i < futures.size(); ++i) {

            int result = futures[i].get();
            std::cout << std::format("Task{} result = {}, Status = {}\n", task_infos[i]->task_id, result, static_cast<int>(task_infos[i]->status));
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "Stop ThreadPool" << std::endl;
    };


    test_submit_with_info();


    return 0;
}
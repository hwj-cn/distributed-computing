#include "ThreadPool.h"
#include <iostream>
#include <format>

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this]() { this->workerLoop(); });
    }
}

ThreadPool::~ThreadPool() {
    stop_ = true;

    for (size_t i = 0; i < workers_.size(); ++i) {
        task_queue_.push([] {});
    }

    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::workerLoop() {
    while (!stop_) {
        std::function<void()> task;
        task_queue_.wait_and_pop(task);

        if (task) {
            try {
                task();  // 执行任务
            } catch (const std::exception& e) {
                std::cerr << std::format("Task exception: {}\n", e.what());
            }
        }
    }
}
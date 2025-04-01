#include "ThreadPool.h"
#include <iostream>
#include <format>

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this]() { this->workerLoop(); });
    }
}

ThreadPool::~ThreadPool() {
    
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

TaskInfo ThreadPool::submit(std::function<void()> f) {
    TaskInfo info;

    info.task_id = ++task_id_counter_;
    info.node_id = "localhost";
    info.submit_time = std::chrono::system_clock::now();

    auto task = [f, info]() mutable {
        auto start = std::chrono::system_clock::now();

        f();

        auto end = std::chrono::system_clock::now();
        info.exec_duration = end - start;
        // 后续可加 result 存储逻辑
        // 此处 info 是副本，返回不了主线程，可后面加回调或 future
    };


    task_queue_.push(task);

    return info;
}
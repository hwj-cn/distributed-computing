#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include <string>
#include <chrono>
#include <any>
#include "SafeQueue.h"

struct TaskInfo {
    size_t task_id;
    std::string node_id;    // 机器 ID
    std::chrono::system_clock::time_point submit_time;
    std::chrono::system_clock::duration exec_duration;
    std::any result;
};

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();  // 后面再补上

    // TODO: submit 方法，返回任务 ID 或 future
    TaskInfo submit(std::function<void()> f);



private:
    void workerLoop();  // 每个线程执行的主循环

    std::vector<std::thread> workers_;
    SafeQueue<std::function<void()>> task_queue_;

    std::atomic<bool> stop_;  // 控制线程退出

    std::atomic<int> task_id_counter_{0};
};

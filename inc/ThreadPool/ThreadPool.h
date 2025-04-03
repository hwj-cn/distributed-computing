#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include <string>
#include <chrono>
#include <any>
#include <future>

#include "SafeQueue.h"
#include "TaskInfo.h"


class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    // TODO: submit 方法，返回任务 ID 或 future
    // TaskInfo submit(std::function<void()> f);
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>> {
        
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        task_queue_.push([task]() { (*task)(); });

        return res;
    }

    template<typename F, typename... Args>
    auto submit_with_info(F&& f, Args&&... args)
        -> std::pair<
            std::shared_ptr<TaskInfo>,
            std::future<std::invoke_result_t<F, Args...>>
        > {

        using return_type = std::invoke_result_t<F, Args...>;

        auto task_info = std::make_shared<TaskInfo>();
        task_info->task_id = task_id_counter_.fetch_add(1);
        task_info->status = TaskStatus::Submitted;
        task_info->submit_time = std::chrono::system_clock::now();


        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();

        task_queue_.push([task, task_info]() {
            auto start = std::chrono::system_clock::now();
            task_info->status = TaskStatus::Running;

            try {
                (*task)();
                task_info->status = TaskStatus::Completed;
            } catch (const std::exception& e) {
                task_info->status = TaskStatus::Failed;
                task_info->error = e.what();
            }

            auto end = std::chrono::system_clock::now();
            task_info->exec_duration = end - start;
        });

        return {task_info, std::move(res)};
    }

private:
    void workerLoop();  // 每个线程执行的主循环

    std::vector<std::thread> workers_;

    SafeQueue<std::function<void()>> task_queue_;

    std::atomic<bool> stop_;  // 控制线程退出

    std::atomic<int> task_id_counter_{0};
};
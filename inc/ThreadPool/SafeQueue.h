#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class SafeQueue {
public:
    void push(const T& item) {
        std::lock_guard<std::mutex> lock_(mutex_);

        queue_.push(item);

        not_empty_.notify_one();
    }

    bool try_pop(T& item) {
        std::lock_guard<std::mutex> lock_(mutex_);

        if (queue_.empty()) return false;

        item = queue_.front();
        queue_.pop();
        return true;
    }

    void wait_and_pop(T& item) {
        std::unique_lock<std::mutex> lock_(mutex_);

        not_empty_.wait(lock_, [&]() { return !queue_.empty(); });

        item = queue_.front();
        queue_.pop();
    }
    
    
    bool empty() const {
        std::lock_guard<std::mutex> lock_(mutex_);

        return queue_.empty();
    }


private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable not_empty_;
};
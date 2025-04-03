#pragma once

enum class TaskStatus {
    Submitted,
    Running,
    Completed,
    Failed
};

struct TaskInfo {
    size_t task_id;
    TaskStatus status;
    std::string node_id;    // 机器 ID
    std::chrono::system_clock::time_point submit_time;
    std::chrono::system_clock::duration exec_duration;
    std::any result;
    std::string error;
};
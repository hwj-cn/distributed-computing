cmake_minimum_required(VERSION 3.10)
project(DistributedComputingSystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(inc)

add_library(threadpool
    src/ThreadPool/ThreadPool.cpp
)

add_executable(main main.cpp)
target_link_libraries(main threadpool)
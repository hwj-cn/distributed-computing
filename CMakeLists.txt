cmake_minimum_required(VERSION 3.10)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    main
    main.cpp
    ThreadPool.cpp
)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpfullversion -dumpversion
    OUTPUT_VARIABLE GCC_VERSION)
    string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${GCC_VERSION})
    list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    list(GET GCC_VERSION_COMPONENTS 1 GCC_MINOR)

    message(STATUS "cmake version=${CMAKE_VERSION}")

    set(GCC_VERSION "${GCC_MAJOR}.${GCC_MINOR}")
    message(STATUS "gcc version=${GCC_VERSION}")
endif()
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES x86_64)

project(raize)

include_directories(include)

set(SOURCE_FILES
        source/processor_sync.cpp
        source/scheduler.cpp
        source/task_processor.cpp
        source/task_provider.cpp)

set(INCLUDE_FILES
        include/execution_context.h
        include/performance_timer.h
        include/processor_sync.h
        include/scheduler.h
        include/task_info.h
        include/task_processor.h
        include/task_provider.h
        include/thread_command.h)

add_library(raize ${SOURCE_FILES} ${INCLUDE_FILES})

add_subdirectory(external)
add_subdirectory(tests)

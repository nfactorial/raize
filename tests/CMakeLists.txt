project(raize_tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(raize_tests
        scheduler_test.cpp
        task_provider_test.cpp
        )

target_link_libraries(raize_tests gtest gtest_main)
target_link_libraries(raize_tests raize)

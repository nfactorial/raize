//
// Copyright 2017 nfactorial
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <chrono>
#include <thread>
#include <atomic>

#include "gtest/gtest.h"
#include "scheduler.h"

// Tests the scheduler with a single task. Ensures the scheduler behaves correctly if
// it doesn't make use of all the available threads.

std::atomic<size_t> taskCounter(0);

static void TestTask_ExecuteFunc()
{
    std::chrono::milliseconds duration(5);
    std::this_thread::sleep_for(duration);

    taskCounter++;
}

// This test task sleeps for long enough to cause the executor to time out.
static void TestTask_TimeoutFunc()
{
    std::chrono::milliseconds duration( 100 );
    std::this_thread::sleep_for(duration);

    taskCounter++;
}

TEST(Scheduler, StartupShutdown) {
    raize::Scheduler scheduler;

    EXPECT_TRUE(scheduler.initialize());
    EXPECT_EQ(RAIZE_SCHEDULER_MAXIMUM_THREADS, scheduler.getThreadCount());
    scheduler.shutdown();
}

// Basic test for raize task scheduler, this test starts up raize and
// tells it to execute without adding any tasks for processing.
// Ensures raize behaves correctly when no tasks are present.
TEST(Scheduler, initialize) {
    raize::Scheduler scheduler;

    taskCounter.store(0);

    EXPECT_TRUE(scheduler.initialize());
    EXPECT_TRUE(scheduler.execute());
    EXPECT_EQ(taskCounter, 0);

    scheduler.shutdown();
}

TEST(Scheduler, TooManyTasks) {
    raize::Scheduler scheduler;

    EXPECT_TRUE(scheduler.initialize());

    const size_t maximumTasks = scheduler.getMaximumTasks();

    for (size_t loop = 0; loop < maximumTasks; ++loop) {
        EXPECT_TRUE(scheduler.createTask(TestTask_ExecuteFunc));
    }

    EXPECT_FALSE(scheduler.createTask(TestTask_ExecuteFunc));
    scheduler.shutdown();
}

// NOTE: The API for creating tasks is only in the preliminary stages and will likely change
//       significantly in the future.

TEST(Scheduler, SingleTask) {
    raize::Scheduler scheduler;

    taskCounter.store(0);

    EXPECT_TRUE(scheduler.initialize());
    EXPECT_TRUE(scheduler.createTask(TestTask_ExecuteFunc));
    EXPECT_TRUE(scheduler.execute());
    EXPECT_EQ(taskCounter, 1);

    scheduler.shutdown();
}

// NOTE: The API for creating tasks is only in the preliminary stages and will likely change
//       significantly in the future.

/* TODO: The timeout works correctly, but we do not terminate the threads that have hung at the moment.
TEST(Scheduler, TimeoutTask) {
    raize::Scheduler scheduler;

    EXPECT_TRUE(scheduler.initialize());
    EXPECT_TRUE(scheduler.createTask(TestTask_TimeoutFunc));
    EXPECT_FALSE(scheduler.execute(5));
    scheduler.shutdown();
}
*/

// NOTE: The API for creating tasks is only in the preliminary stages and will likely change
//       significantly in the future.

TEST(Scheduler, MultiTask) {
    raize::Scheduler scheduler;

    EXPECT_TRUE(scheduler.initialize());

    const size_t threadCount = scheduler.getThreadCount();

    unsigned int taskCount = 0;

    for (size_t loop = 0; loop < threadCount; ++loop) {
        EXPECT_TRUE(scheduler.createTask(TestTask_ExecuteFunc));
        taskCount++;
    }

    taskCounter.store(0);

    EXPECT_TRUE(scheduler.execute());
    EXPECT_EQ(taskCounter, threadCount);

    scheduler.shutdown();
}

TEST(Scheduler, MassTask) {
    raize::Scheduler scheduler;

    EXPECT_TRUE(scheduler.initialize());

    const size_t threadCount = scheduler.getThreadCount();
    const unsigned int taskCount = static_cast< unsigned int >(threadCount * 30);

    for (size_t loop = 0; loop < taskCount; ++loop) {
        EXPECT_TRUE(scheduler.createTask(TestTask_ExecuteFunc));
    }

    taskCounter.store(0);

    EXPECT_TRUE(scheduler.execute());
    EXPECT_EQ(taskCounter, threadCount * 30);

    scheduler.shutdown();
}

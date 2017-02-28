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

#include "gtest/gtest.h"

#include "task_info.h"
#include "task_provider.h"

// Tests the scheduler with a single task. Ensures the scheduler behaves correctly if
// it doesn't make use of all the available threads.

static void TestTask_ExecuteFunc1()
{
    std::chrono::milliseconds duration( 10 );
    std::this_thread::sleep_for( duration );
}

static void TestTask_ExecuteFunc2()
{
    std::chrono::milliseconds duration( 10 );
    std::this_thread::sleep_for( duration );
}

TEST(TaskProvider, Construction) {
    raize::TaskProvider taskProvider;

    EXPECT_EQ(0, taskProvider.getMaximumTasks());
    EXPECT_EQ(nullptr, taskProvider.nextTask());
}

TEST(TaskProvider, Initialize) {
    raize::TaskProvider taskProvider;

    EXPECT_FALSE(taskProvider.initialize(0));
    EXPECT_TRUE(taskProvider.initialize(100));
    EXPECT_EQ(100, taskProvider.getMaximumTasks());
}

TEST(TaskProvider, AddTask) {
    raize::TaskProvider taskProvider;

    EXPECT_TRUE(taskProvider.initialize(1));
    EXPECT_EQ(1, taskProvider.getMaximumTasks());
    EXPECT_TRUE(taskProvider.addTask(TestTask_ExecuteFunc1));
    EXPECT_FALSE(taskProvider.addTask(TestTask_ExecuteFunc1));
}

TEST(TaskProvider, NextTask) {
    raize::TaskProvider taskProvider;

    EXPECT_TRUE(taskProvider.initialize(2));
    EXPECT_EQ(2, taskProvider.getMaximumTasks());
    EXPECT_TRUE(taskProvider.addTask(TestTask_ExecuteFunc1));
    EXPECT_TRUE(taskProvider.addTask(TestTask_ExecuteFunc2));

    EXPECT_EQ(2, taskProvider.onBeginProcessing());

    // NOTE: We might not be guaranteed order in the future

    const raize::TaskInfo *infoA = taskProvider.nextTask();
    const raize::TaskInfo *infoB = taskProvider.nextTask();

    EXPECT_EQ(nullptr, taskProvider.nextTask());

    EXPECT_TRUE(TestTask_ExecuteFunc1 == infoA->execute);
    EXPECT_TRUE(TestTask_ExecuteFunc2 == infoB->execute);
}

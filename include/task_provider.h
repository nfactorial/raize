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

#if !defined( TASK_PROVIDER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define TASK_PROVIDER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

#include <cstdio>
#include <vector>
#include <atomic>

#include "task_info.h"


// -----------------------------------------------------------------------------------

namespace raize {
    //! \brief Provides an API for obtaining a tasks to be processed by a thread.
    //!
    //! The task provider has a maximum number of tasks it can contain and nomore. This should
    //! be set to some definite maximum, the reason for this is to avoid fragmenting the heap
    //! at run-time.
    //!
    class TaskProvider {
        typedef std::vector<TaskInfo> TaskList;
        typedef TaskList::iterator TaskIterator;

    public:
        TaskProvider();
        ~TaskProvider();

        void shutdown();

        bool initialize(size_t taskCapacity);

        bool addTask(TaskExecuteFunction executeFunc);

        void onEndProcessing();
        size_t onBeginProcessing();

        TaskInfo *nextTask();

        size_t getMaximumTasks() const;

    private:
        std::atomic<unsigned int> m_taskAcquire;
        size_t m_nextTask;
        TaskList m_tasks;

        TaskProvider(const TaskProvider &other);

        TaskProvider &operator=(const TaskProvider &other);
    };

    //! \brief  Retrieves the maximum number of tasks that may be queued within the task provider.
    //! \return The maximum number of tasks that may be queued within the task provider.
    inline size_t TaskProvider::getMaximumTasks() const {
        return m_tasks.capacity();
    }
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( TASK_PROVIDER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

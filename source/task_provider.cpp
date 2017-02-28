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

#include "task_provider.h"


// -----------------------------------------------------------------------------------

namespace raize {
    // -----------------------------------------------------------------------------------

    TaskProvider::TaskProvider()
    : m_nextTask(0)
    {
        m_taskAcquire.store(0);
    }

    TaskProvider::~TaskProvider() {
    }


    //! \brief  Prepares the task provider for use by the running application.
    //! \param  taskCapacity [in] -
    //!         The maximum number of tasks that may be contained within the provider at one time.
    //! \return <em>True</em> if the provider initialized successfully otherwise <em>false</em>.
    bool TaskProvider::initialize(size_t taskCapacity) {
        if (taskCapacity > 0) {
            m_tasks.reserve(taskCapacity);
            return true;
        }

        return false;
    }

    //! \brief  
    void TaskProvider::shutdown() {
        m_nextTask = 0;

        m_tasks.clear();
    }


    //! \brief  Adds a new task to the provider, to be executed during the next processing frame.
    //! \param  executeFunc [in] -
    //!         The function that implements the processing necessary for the task.
    //! \return <em>True</em> if the task was added sucessfully otherwise <em>false</em>.
    bool TaskProvider::addTask(TaskExecuteFunction executeFunc) {
        if (m_tasks.size() < m_tasks.capacity()) {
            TaskInfo taskInfo;

            taskInfo.executionSpeed = 0;
            taskInfo.execute = executeFunc;

            m_tasks.push_back(taskInfo);

            return true;
        }

        return false;
    }

    //! \brief  Called by the scheduler when it is about to begin processing tasks.
    //! \return The number of tasks that are awaiting processing.
    size_t TaskProvider::onBeginProcessing() {
        m_nextTask = 0;
        return m_tasks.size();
    }


    //! \brief  Called by the scheduler when it has completed processing the queued tasks.
    void TaskProvider::onEndProcessing() {
    }


    //! \brief  Retrieves the next task to be procesed.
    //! \return Pointer to the task to be processed by the calling thread, if no tasks remain this method returns <em>nullptr</em>.
    TaskInfo *TaskProvider::nextTask() {
        TaskInfo *taskInfo = nullptr;

        // Ensure we're the only thread accessing our data, we do not want to use a mutex lock here
        unsigned int acquireExpected = 0;
        while (!m_taskAcquire.compare_exchange_weak(acquireExpected, 1)) {
            // QUERY: See if there's an alternate we can use that doesn't need us to reset this variable
            acquireExpected = 0;
        }

        // As long as we have tasks left to process
        if (m_nextTask < m_tasks.size()) {
            taskInfo = &m_tasks[m_nextTask++];
        }

        // Allow other threads access to the task list
        m_taskAcquire.store(0);

        return taskInfo;
    }


    // -----------------------------------------------------------------------------------

} // namespace raize

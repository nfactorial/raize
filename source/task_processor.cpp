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

#include <cassert>
#include "task_processor.h"
#include "performance_timer.h"
#include "processor_sync.h"
#include "task_provider.h"


// -----------------------------------------------------------------------------------

namespace raize {
    // -----------------------------------------------------------------------------------

    TaskProcessor::TaskProcessor()
    : m_syncObject(nullptr)
    {
        m_threadCommand = {kThreadCommand_None, nullptr};

        m_executionContext.contextId = 0;
        m_executionContext.executionSpeed = 0;
        m_executionContext.tasksProcessed = 0;
    }

    TaskProcessor::~TaskProcessor() {
    }


    //! \brief  Prepares the thread for use by the host application.
    //! \param  executionContext [in] -
    //!         Description of thhe the execution environment the thread will be executing in.
    //! \return <em>True</em> if the thread initialized successfully otherwise <em>false</em>.
    bool TaskProcessor::initialize(const ExecutionContext &executionContext, ProcessorSync *syncObject) {
        assert(nullptr != syncObject);

        m_syncObject = syncObject;
        m_executionContext = executionContext;

        m_thread = std::thread(TaskProcessor::threadEntry, this);

        return true;
    }


    //! \brief  Joins with the thread contained within the TaskProcessor object.
    void TaskProcessor::join() {
        m_thread.join();
    }


    //! \brief  Static method for the thread entry point.
    //! \param  self [in] -
    //!         Pointer to the TaskProcessor instance we belong to.
    void TaskProcessor::threadEntry(TaskProcessor *self) {
        assert(nullptr != self);
        self->threadExecute();
    }


    //! \brief  Posts a new command to the thread for processing.
    //! \param  threadCommand [in] -
    //!         The command to be performed by the thread.
    void TaskProcessor::postCommand(const ThreadCommand &threadCommand) {
        assert(kThreadCommand_None != threadCommand.id);

        m_threadCommand = threadCommand;
    }


    //! \brief  Main thread processing function, performs the current queued thread command then waits for the next one. 
    void TaskProcessor::threadExecute() {
        m_syncObject->notifyReady();

        while (kThreadCommand_Exit != m_threadCommand.id) {
            m_syncObject->waitExecute();

            switch (m_threadCommand.id) {
                case kThreadCommand_None:
                    // Should never get here
                    break;

                case kThreadCommand_Execute:
                    executeTaskList();
                    break;

                case kThreadCommand_Exit:
                    break;
            }
        }

        m_threadCommand = {kThreadCommand_None, nullptr};

        m_executionContext.contextId = 0;
        m_executionContext.executionSpeed = 0;
        m_executionContext.tasksProcessed = 0;
    }


    //! \brief  Processes as many tasks as the task provider instance can supply us with.
    void TaskProcessor::executeTaskList() {
        const PerformanceTimer timer;

        m_executionContext.tasksProcessed = 0;

        assert(nullptr != m_threadCommand.taskProvider);
        while (executeTask(m_threadCommand.taskProvider->nextTask())) {
            m_executionContext.tasksProcessed++;
        }

        m_executionContext.executionSpeed = timer.getElapsedTimeMilli();

        m_threadCommand.id = kThreadCommand_None;
        m_threadCommand.taskProvider = nullptr;

        m_syncObject->notifyComplete();     // Notify command issuer we have completed.
    }


    //! \brief  Performs a single tasks operation within our thread.
    //! \param  taskInfo [in] -
    //!         Description of the task to be executed, if this parameter is <em>nullptr</em> this method will fail.
    //! \return <em>True</em> if the task was processed successfully otherwise <em>false</em>
    bool TaskProcessor::executeTask(TaskInfo *taskInfo) {
        if (nullptr != taskInfo) {
            // TODO: This is where we prepare the tasks parameters for use, before sending them along with the execute() call below.

            const PerformanceTimer timer;

            taskInfo->execute();
            taskInfo->executionSpeed = timer.getElapsedTimeMilli();

            return true;
        }

        return false;
    }


    // -----------------------------------------------------------------------------------

} // namespace raize

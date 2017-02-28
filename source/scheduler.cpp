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
#include "scheduler.h"
#include "performance_timer.h"


// -----------------------------------------------------------------------------------

namespace raize {
    //! This constant is used to timeout the 'wait for completion' operation. If the
    //! execution of a task list extends beyond this value, we exit and complain.
    //! This is to catch unexpected hangs within the execution frame, in the event of
    //! such a timeout, raize will report the tasks that took too long to complete.
    //! Hopefully this will raise dependency errors in the scheduling during development
    //! or direct the user to faulty task implementations.
    static const unsigned int kRaizeExecutionTimeout = 1000;     // Milliseconds
    static const size_t kRaizeDefaultMaximumTasks = 256;         // TODO: Allow developer to specify


    // -----------------------------------------------------------------------------------

    Scheduler::Scheduler()
    : m_executionTime(0)
    , m_threadCount(0)
    {
    }

    Scheduler::~Scheduler() {
        shutdown();
    }

    //! \brief  Prepares the scheduler for use by the application, this method defaults to using the maximum number of threads.
    //! \return <em>True</em> if the scheduler initialized successfuly otherwise <em>false</em>.
    bool Scheduler::initialize() {
        return initialize(RAIZE_SCHEDULER_MAXIMUM_THREADS);
    }


    //! \brief  Prepares the scheduler for use by the application.
    //! \param  threadCount [in] -
    //!         The number of threads the scheduler will make use of, this must be less than or equal to RAIZE_SCHEDULER_MAXIMUM_THREADS.
    //! \return <em>True</em> if the scheduler initializes successfully otherwise <em>false</em>.
    bool Scheduler::initialize(size_t threadCount) {
        assert(0 == m_threadCount);
        assert(0 != threadCount);

        if (0 != m_threadCount) {
            //Log( "TaskProcessorCollection::initialize - Collection was already initialized.\n" );
            return false;
        }

        m_syncObject.initialize(threadCount);

        if (!m_taskProvider.initialize(kRaizeDefaultMaximumTasks)) {
            return false;
        }

        for (; m_threadCount < threadCount; ++m_threadCount) {
            ExecutionContext executionContext;

            executionContext.contextId = static_cast< unsigned int >(m_threadCount);
            executionContext.executionSpeed = 0;
            executionContext.tasksProcessed = 0;

            if (!m_taskProcessors[m_threadCount].initialize(executionContext, &m_syncObject)) {
                shutdown();
                return false;
            }
        }

        m_syncObject.waitReady();
        return true;
    }


    //! \brief  Terminates all threads and closes the scheduler.
    void Scheduler::shutdown() {
        if (0 != m_threadCount) {
            const ThreadCommand threadCommand = {kThreadCommand_Exit, nullptr};

            // Send exit command to all child threads
            for (size_t loop = 0; loop < m_threadCount; ++loop)
                m_taskProcessors[loop].postCommand(threadCommand);

            m_syncObject.notifyExit();

            // And wait for them to exit
            for (size_t loop = 0; loop < m_threadCount; ++loop)
                m_taskProcessors[loop].join();

            m_threadCount = 0;

            m_taskProvider.shutdown();
        }
    }


    //! \brief  Creates a new task for processing within the scheduler.
    //! \param  taskFunction [in] -
    //!         The function to be called when the task is to be executed.
    //! \return <i>True</i> if the task was successfully created otherwise <i>false</i>.
    bool Scheduler::createTask(TaskExecuteFunction taskFunction) {
        return m_taskProvider.addTask(taskFunction);
    }

    //! \brief  Retrieves the maximum number of tasks supported by the scheduler instance.
    //! \return The maximum number of tasks that may be queued within the scheduler.
    size_t Scheduler::getMaximumTasks() const {
        return m_taskProvider.getMaximumTasks();
    }

    //! \brief  Begins processing of the current task queue.
    //! \return <em>True</em> if processing completed successfully otherwise <em>false</em> if an issue occurred during processing.
    bool Scheduler::execute() {
        execute(kRaizeExecutionTimeout);
    }

    //! \brief  Begins processing of the current task queue with a specified timeout value.
    //! \param  timeOut [in] -
    //!         Time (in milliseconds) allowed for a task to complete executing before being considered hung.
    //! \return <em>True</em> if processing completed successfully otherwise <em>false</em> if an issue occurred during processing.
    bool Scheduler::execute(uint64_t timeOut) {
        assert(0 != m_threadCount);

        PerformanceTimer timer;

        const size_t taskCount = m_taskProvider.onBeginProcessing();
        if (0 != taskCount) {
            if (!executeTasks(m_taskProvider, timeOut)) {
                // TODO: If we timed out, we report tasks that are currently being processed.
                // Also log any tasks that took an extraordinary amount of time to complete.
                // The user should be able to disable the timeout at compile time, so that
                // we (optionally) don't fail at run-time in the wild.

                shutdown();     // We're no longer valid for executing tasks, we shutdown to avoid continually failing. We might restart ourselves in the future if desirable

                return false;
            }

            m_taskProvider.onEndProcessing();
        }

        m_executionTime = timer.getElapsedTimeMilli();
        return true;
    }


    //! \brief  Tells all task processing threads to begin processing tasks.
    //! \param  taskProvider [in] -
    //!         The TaskProvider implementation that will supply tasks to all child threads.
    //! \param  timeOut [in] -
    //!         The longest duration to wait before the execute operation will timeout, if this value is 0 the scheduler will wait indefinitely.
    //! \return <em>True</em> if the processing threads were started successfully otherwise <em>false</em>.
    bool Scheduler::executeTasks(TaskProvider &taskProvider, uint64_t timeOut) {
        assert(0 != m_threadCount);

        const ThreadCommand threadCommand = {kThreadCommand_Execute, &taskProvider};

        for (size_t loop = 0; loop < m_threadCount; ++loop)
            m_taskProcessors[loop].postCommand(threadCommand);

        return m_syncObject.notifyExecute(timeOut);
    }

    // -----------------------------------------------------------------------------------

} //namespace raize

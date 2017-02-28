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

#if !defined( TASK_PROCESSOR_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define TASK_PROCESSOR_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

#include <thread>

#include "execution_context.h"


// -----------------------------------------------------------------------------------

namespace raize {
    struct TaskInfo;

    class ProcessorSync;

    //! \brief Manages the processing of a single thread within the scheduler.
    class TaskProcessor {
    public:
        TaskProcessor();
        ~TaskProcessor();

        void join();
        bool initialize(const ExecutionContext &executionContext, ProcessorSync *syncObject);

        void postCommand(const ThreadCommand &threadCommand);

    private:
        void executeTaskList();

        bool executeTask(TaskInfo *taskInfo);

        void threadExecute();

        static void threadEntry(TaskProcessor *self);

    private:
        ThreadCommand m_threadCommand;  //!< The current operation being performed by this execution context
        ExecutionContext m_executionContext;
        ProcessorSync *m_syncObject;

        std::thread m_thread;

        TaskProcessor(const TaskProcessor &other);

        TaskProcessor &operator=(const TaskProcessor &other);
    };
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( TASK_PROCESSOR_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

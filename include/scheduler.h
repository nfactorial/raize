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

#if !defined( SCHEDULER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define SCHEDULER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

#include <stdint.h>
#include <array>

#include "processor_sync.h"
#include "task_processor.h"
#include "task_provider.h"


// -----------------------------------------------------------------------------------

//! This define specifies the maximum number of threads the scheduler will support
//! at run time, you can override it by redefining it here or defining it as part
//! of your build configuration.
#if !defined( RAIZE_SCHEDULER_MAXIMUM_THREADS )
    #define RAIZE_SCHEDULER_MAXIMUM_THREADS    4
#endif //!defined( RAIZE_SCHEDULER_MAXIMUM_THREADS )


// -----------------------------------------------------------------------------------

//!< \brief Class responsible for managing all the threads and tasks within the application.
//!
//! The Raize scheduler supports a maximum number of threads as defined by the RAIZE_SCHEDULER_MAXIMUM_THREADS
//! define. This define allows the application to specify a per-platform constant for the number of supported
//! threads.
//!
//! On platforms that have a varying number of of threads (such as home desktop machines) the define should be
//! set to a reasonable maximum number. The title is then free to determine the number of cores it wishes
//! to make use of at run-time and supply the correct amount to the initialize method.
//!
//! For example, a title may reasonably specify a maximum of 16 threads on a home machine, then determine
//! the host machine has only 2 cores at run-time.
//!
//! \code
//! void exmaple()
//! {
//!     raize::Scheduler scheduler;
//!
//!     scheduler.initialize( min( getNumProcessorCores(), RAIZE_SCHEDULER_MAXIMUM_THREADS ) );
//! }
//! \endcode
//!
//! Note that it is left to the application author to implement getNumProcessorCores in the above example.
//!
namespace raize {
    class Scheduler {
        typedef std::array<TaskProcessor, RAIZE_SCHEDULER_MAXIMUM_THREADS> TaskProcessorList;

    public:
        Scheduler();
        ~Scheduler();

        bool initialize();
        bool initialize(size_t threadCount);

        void shutdown();

        bool execute();
        bool execute(uint64_t timeOut);

        bool createTask(TaskExecuteFunction taskFunction);

        size_t getThreadCount() const;
        size_t getMaximumTasks() const;
        uint64_t getExecutionTime() const;

    private:
        bool executeTasks(TaskProvider &taskProvider, uint64_t timeOut);

    private:
        uint64_t m_executionTime;            // How long did it take to process the entire graph (in milliseconds)
        size_t m_threadCount;              // Number of threads in use

        TaskProvider m_taskProvider;
        ProcessorSync m_syncObject;
        TaskProcessorList m_taskProcessors;

        Scheduler(const Scheduler &other);

        Scheduler &operator=(const Scheduler &other);
    };


    //! \brief  Retrieves the number of threads currently in use by the scheduler.
    //! \return The number of threads currently in use by the scheduler.
    inline size_t Scheduler::getThreadCount() const {
        return m_threadCount;
    }


    //! \brief  Returns the time (in milliseconds) taken to execute the previous execution phase of the task graph.
    //! \return The time (in milliseconds) the scheduler took to complete the last execution phase.
    inline uint64_t Scheduler::getExecutionTime() const {
        return m_executionTime;
    }
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( SCHEDULER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

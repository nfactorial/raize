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

#include "../include/scheduler.h"
#include "../include/processor_sync.h"


// -----------------------------------------------------------------------------------

namespace raize {
    // -----------------------------------------------------------------------------------

    ProcessorSync::ProcessorSync()
    : m_completionCounter(0)
    , m_readyCounter(0)
    , m_totalThreads(0)
    {
    }

    ProcessorSync::~ProcessorSync() {
    }


    //! \brief Prepares the synchronization object for use by the application.
    //! \param	threadCount [in] -
    //!			The number of worker threads that will be managed by the synchronization object.
    //! \return	<em>True</em> if the synchronization object initialized successfully otherwise <em>false</em>.
    bool ProcessorSync::initialize(size_t threadCount) {
        if (0 == m_totalThreads) {
            m_totalThreads = threadCount;
            m_readyCounter = 0;
            m_completionCounter = 0;
            return true;
        }

        return false;
    }

    //! \brief  Notifies the synchronization object that a task processor is ready for use.
    void ProcessorSync::notifyReady() {
        std::unique_lock<std::mutex> lock(m_readyMutex);
        if (++m_readyCounter == m_totalThreads) {
            m_readyCondition.notify_one();
        }
    }

    //! \brief	Called when an Exit command has been sent to the waiting threads.
    //!
    //! Calling code should use this method instead of notifyExecute() for the Exit command as
    //! notifyExit() will <em>not</em> wait for the completion condition to be raised.
    void ProcessorSync::notifyExit() {
        std::unique_lock<std::mutex> lock(m_executeMutex);
        m_completionCounter = 0;
        m_executeCondition.notify_all();
    }

    //! \brief	Signals that a thread has completed processing of its current operation, when all threads have completed the completion signal is raised.
    void ProcessorSync::notifyComplete() {
        std::unique_lock<std::mutex> lock(m_executeMutex);
        if (++m_completionCounter == m_totalThreads) {
            m_completionCondition.notify_one();
        }
    }

    //! \brief	This method waits until an execute loop is issued for a command on the dependent threads.
    //!
    //! Worker threads call waitExecute() when they have no work to be performed, this will put them
    //! to sleep until a new operation has been provided.
    void ProcessorSync::waitExecute() {
        std::unique_lock<std::mutex> lock(m_executeMutex);
        m_executeCondition.wait(lock);
    }

    //! \brief	Sends a signal to all worker threads that a new command has been issued for processing, then waits for them to complete with a timeout.
    //! \param	timeOut [in] -
    //!			Maximum duration (in milliseconds) the sync object should wait before giving up, if this value is 0 the sync object will wait indefinitely.
    //!
    //! If the threads do not complete their operation before the time specified in the timeOut variable expires, this method will return
    //! false. This allows the developer to discover unexpected hangs during development, it is recommended not to use this method
    //! in development code as unexpected behaviour in the host operating system may unexpectedly cause spikes in execution time.
    //!
    //! \return	<em>True</em> if the threads completed successfully otherwise <em>false</em>
    bool ProcessorSync::notifyExecute(uint64_t timeOut) {
        std::unique_lock<std::mutex> lock(m_executeMutex);
        m_completionCounter = 0;
        m_executeCondition.notify_all();

        if (0 != timeOut)
            return (std::cv_status::no_timeout ==
                    m_completionCondition.wait_for(lock, std::chrono::milliseconds(timeOut)));

        m_completionCondition.wait(lock);
        return true;
    }

    //! \brief Pauses the calling thread until all child threads within the raize scheduler have informed us they are ready.
    void ProcessorSync::waitReady() {
        std::unique_lock<std::mutex> lock(m_readyMutex);
        while (m_readyCounter != m_totalThreads) {
            m_readyCondition.wait(lock);
        }
    }

    // -----------------------------------------------------------------------------------

} //namespace raize

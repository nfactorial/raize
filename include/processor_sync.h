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

#if !defined( PROCESSOR_SYNC_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define PROCESSOR_SYNC_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

#include <condition_variable>
#include <atomic>
#include <mutex>


// -----------------------------------------------------------------------------------

namespace raize {
    //! \brief  Implements the synchronization primitives used to manage the worker threads.
    class ProcessorSync {
    public:
        ProcessorSync();
        ~ProcessorSync();

        bool initialize(size_t threadCount);

        void notifyExit();
        void notifyComplete();
        bool notifyExecute(uint64_t timeOut);

        void waitReady();
        void waitExecute();

        void notifyReady();

    private:
        std::mutex m_executeMutex;
        std::mutex m_readyMutex;

        std::condition_variable m_readyCondition;
        std::condition_variable m_completionCondition;
        std::condition_variable m_executeCondition;

        size_t m_completionCounter;
        size_t m_readyCounter;
        size_t m_totalThreads;

        ProcessorSync(const ProcessorSync &other);

        ProcessorSync &operator=(const ProcessorSync &other);
    };
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( PROCESSOR_SYNC_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

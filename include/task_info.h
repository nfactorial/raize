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

#if !defined( TASK_INFO_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define TASK_INFO_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

#include <stdint.h>
#include <atomic>


// -----------------------------------------------------------------------------------

namespace raize {
    typedef void ( *TaskExecuteFunction )();

    //! \brief  Defines a single task registered with the scheduler.
    //!
    //! This will be expanded on greatly in the future, currently it's a simple function
    //! call. But we'll want a dependency graph and parameters supported eventually.
    //! This will start to be fleshed out once the core framework is processing these
    //! simple tasks.
    struct TaskInfo {
        uint64_t executionSpeed; //!< How longs did the task take to complete
        TaskExecuteFunction execute;
    };
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( TASK_INFO_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

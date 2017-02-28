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

#if !defined( EXECUTION_CONTEXT_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define EXECUTION_CONTEXT_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

#include <condition_variable>
#include <stdint.h>

#include "thread_command.h"


// -----------------------------------------------------------------------------------

namespace raize {
    //! \brief	An execution context describes a processing unit within the scheduler.
    //!
    //! Whilst an execution unit is typically an individual thread, it should make no
    //! assumptions about this.
    //! The context id is specifically a raize identifier and should not be considered
    //! to be the processing core or any hardware related value.
    //!
    struct ExecutionContext {
        unsigned int contextId;      //!< Identifier for this execution context.
        unsigned int tasksProcessed; //!< Number of tasks we processed this frame
        uint64_t executionSpeed;     //!< How fast did the context take to complete all the supplied tasks in a frame (in milliseconds)
    };
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( EXECUTION_CONTEXT_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

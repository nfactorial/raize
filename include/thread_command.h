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

#if !defined( THREAD_COMMAND_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define THREAD_COMMAND_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

namespace raize {
    class TaskProvider;

    enum kThreadCommand {
        kThreadCommand_None,            //!< No command, thread should do nothing and wait for further instructions
        kThreadCommand_Execute,         //!< Thread should execute as many tasks as provided
        kThreadCommand_Exit,            //!< Thread should terminate
    };

    //! \brief Description of a single operation to be performed by a thread.
    struct ThreadCommand {
        kThreadCommand id;               //!< The command represented by this structure
        TaskProvider *taskProvider;      //!< Object that will supply tasks for the processor
    };
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( THREAD_COMMAND_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

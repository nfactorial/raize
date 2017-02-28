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

#if !defined( PERFORMANCE_TIMER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define PERFORMANCE_TIMER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

#include <chrono>


// -----------------------------------------------------------------------------------

namespace raize {
    class PerformanceTimer {
        typedef std::chrono::high_resolution_clock time_source;

    public:
        PerformanceTimer();

        void reset();

        uint64_t getElapsedTimeNano() const;
        uint64_t getElapsedTimeMilli() const;

    private:
        time_source::time_point m_start;
    };
} // namespace raize


// -----------------------------------------------------------------------------------

#include "performance_timer.inl"


// -----------------------------------------------------------------------------------

#endif //!defined( PERFORMANCE_TIMER_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

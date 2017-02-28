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

#if !defined( PERFORMANCE_TIMER_INL_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )
#define PERFORMANCE_TIMER_INL_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL


// -----------------------------------------------------------------------------------

namespace raize {
    //! \brief  Constructor that begins the performance timer for measuring elapsed time.
    inline PerformanceTimer::PerformanceTimer()
    : m_start(time_source::now())
    {
    }


    //! \brief  Resets the timer so it's measurement begins at the current point in time.
    inline void PerformanceTimer::reset() {
        m_start = time_source::now();
    }


    //! \brief  Returns the number of milliseconds that have elapsed since the performance timer was created.
    //! \return The number of milliseconds that have elapsed since the performance timer was created.
    inline uint64_t PerformanceTimer::getElapsedTimeMilli() const {
        const time_source::time_point current = time_source::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(current - m_start).count();
    }


    //! \brief  Returns the number of nanoseconds that have elapsed since the performance timer was created.
    //! \return The number of nanoseconds that have elapsed since the performance timer was created.
    inline uint64_t PerformanceTimer::getElapsedTimeNano() const {
        const time_source::time_point current = time_source::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(current - m_start).count();
    }
} // namespace raize


// -----------------------------------------------------------------------------------

#endif //!defined( PERFORMANCE_TIMER_INL_HEADER_INCLUDED_FEBRUARY_2017_NFACTORIAL )

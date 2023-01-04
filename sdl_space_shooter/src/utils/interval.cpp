#include "pch.h"
#include "interval.h"

namespace utils
{
    interval& interval::get_instance()
    {
        static interval instance;
        return instance;
    }

    void interval::insert(int64_t interval_seconds, interval_function* interval_func)
    {
        interval_functions.push_back(interval_func);
        intervals[interval_func] = std::make_pair(SDL_GetTicks(), interval_seconds);
    }

    void interval::remove(interval_function* interval_func) {
        std::erase(interval_functions, interval_func);
        intervals.erase(interval_func);
    }

    void interval::update()
    {
        // Update the current time
        current_time = SDL_GetTicks();

        // Call the interval functions
        for (auto& interval_func : interval_functions)
        {
            auto& [current_time_interval, interval_seconds] = intervals[interval_func];

            // Check if the interval has passed
            if (current_time >= current_time_interval)
            {
                // Call the function
                (*interval_func)();
                // Update the last time the function was called
                const int64_t next_time = current_time + (1000 * interval_seconds);
                current_time_interval = next_time;
            }
        }
    }
}

#pragma once

namespace application
{
    using interval_function = std::function<void()>;

    class interval
    {
    public:
        static interval& get_instance();
        void insert(int64_t interval_seconds, interval_function* interval_func);
        void remove(interval_function* interval_func);
        void update();
    private:
        int64_t current_time { 0 };
        std::vector<interval_function*> interval_functions;
        std::unordered_map<interval_function*, std::pair<int64_t, int64_t>> intervals;
        interval() = default;
    };
}

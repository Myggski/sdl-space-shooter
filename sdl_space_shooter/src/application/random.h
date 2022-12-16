#pragma once

#include <random>

namespace application
{
    template<class T>
    using uniform_distribution =
        typename std::conditional<
        std::is_floating_point<T>::value,
        std::uniform_real_distribution<T>,
        typename std::conditional<
        std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        void
        >::type
        >::type;

    template <class T>
    T get_random(T min, T max) {
        std::random_device rd;
        std::ranlux48 gen(rd());
        const uniform_distribution<T> dis(min, max);
        return dis(gen);
    }
}

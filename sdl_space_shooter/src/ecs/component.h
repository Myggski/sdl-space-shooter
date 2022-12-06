#pragma once

#include <cstddef>

namespace ecs
{

    template<typename T, auto Type>
    class component
    {
    public:
        static constexpr auto type = static_cast<std::size_t>(Type);
    };

}
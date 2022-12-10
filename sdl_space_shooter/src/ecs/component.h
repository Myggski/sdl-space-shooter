#pragma once

#include <cstddef>

namespace ecs
{
    enum class component_types
    {
        input,
        position,
        size,
        velocity,
        texture
    };
    template<typename T, auto Type>
    class component
    {
    public:
        static constexpr auto type = static_cast<std::size_t>(Type);
    };
}
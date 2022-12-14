#pragma once

#include <cstddef>

namespace ecs
{
    namespace components
    {
        enum class component_types
        {
            input,
            position,
            size,
            velocity,
            texture,
            box_collider,
            damage,
            health,
            rotation,
            layer,
            removal_timer,
            text,
            points,
        };

        template<typename T, auto Type>
        class component
        {
        public:
            static constexpr auto type = static_cast<std::size_t>(Type);
        };
    }
}

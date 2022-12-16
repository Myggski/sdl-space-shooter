#pragma once

#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace systems
    {
        class physics_system final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            physics_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world);
            void try_update_position(const float dt) const;
        };
    }
}

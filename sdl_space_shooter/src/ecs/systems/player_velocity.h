#pragma once

#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace systems
    {
        class player_velocity final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            player_velocity(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world);

        private:
            void update_velocity(const float dt) const;
        };
    }
}

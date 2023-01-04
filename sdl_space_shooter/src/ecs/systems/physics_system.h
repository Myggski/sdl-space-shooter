#pragma once

#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace components
    {
        struct velocity;
    }

    namespace systems
    {
        class physics_system final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            physics_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world);

        private:
            void player_screen_wrap(ecs::entity entity, ecs::components::velocity& velocity, SDL_FPoint& new_position, const SDL_FPoint& old_position) const;
            void try_update_position(const float dt);
        };
    }
}

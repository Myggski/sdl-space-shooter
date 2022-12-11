#pragma once

#include "ecs/system.h"
#include "collision/spatial_grid.h"
#include "ecs/world.h"

namespace ecs
{
    namespace systems
    {
        class damage_collision final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            damage_collision(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world);
            void check_collision(const float dt) const;
        private:
            const SDL_FRect get_rect_data(entity entity) const;
        };
    }
}

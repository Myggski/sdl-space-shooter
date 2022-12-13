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
            void check_collision(const float dt);
        private:
            std::unordered_map<ecs::entity, std::future<std::set<ecs::entity>>> futures;
            void find_near_entities(const std::vector<entity>& entities);
            std::set<ecs::entity> apply_damage(const ecs::entity entity, const std::set<ecs::entity>& nearby_entities) const;
            const SDL_FRect get_rect_data(entity entity) const;
        };
    }
}

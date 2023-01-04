#pragma once

#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace systems
    {
        class damage_collision final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            damage_collision(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world);

        private:
            void try_collide(const float dt) const;
            std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>> search_nearby_entities(const std::vector<entity>& entities) const;
            std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>> group_search(const std::vector<ecs::entity>& entities) const;
            std::unordered_set<ecs::entity> check_for_collisions(const std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>>& nearby_entities_data) const;
            std::unordered_set<ecs::entity> try_apply_damage(const ecs::entity entity, const std::unordered_set<ecs::entity>& nearby_entities) const;
            void remove_dead_entities( const std::unordered_set<ecs::entity>& dead_entities) const;
            const SDL_FRect get_rect_data(entity entity) const;
        };
    }
}

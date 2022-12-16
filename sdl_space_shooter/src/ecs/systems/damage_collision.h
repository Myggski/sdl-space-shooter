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
            void check_collision(const float dt);
            void on_valid_entity_removed(entity entity) override;
        private:
            void group_search(const std::vector<ecs::entity>& entities);
            std::unordered_map<ecs::entity, std::future<std::unordered_set<ecs::entity>>> nearby_data_async;
            void search_nearby_entities_async(const std::vector<entity>& entities);
            std::unordered_set<ecs::entity> try_apply_damage(const ecs::entity entity, const std::unordered_set<ecs::entity>& nearby_entities) const;
            const SDL_FRect get_rect_data(entity entity) const;
        };
    }
}

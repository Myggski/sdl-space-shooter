#include "pch.h"

#include "damage_collision.h"



#include "ecs/components/position.h"
#include "ecs/components/box_collider.h"
#include "ecs/world.h"
#include "collision/aabb.h"
#include "ecs/components/damage.h"
#include "ecs/components/health.h"

namespace ecs
{
    namespace systems
    {
        const SDL_FRect damage_collision::get_rect_data(entity entity) const
        {
            auto [position, box_collider] = world.get_components<components::position, components::box_collider>(entity);
            return ::collision::get_rect_data(position, box_collider);
        }

        damage_collision::damage_collision(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world)
            : system(world)
        {
            set_all_requirements<components::position, components::box_collider, components::damage>();
            set_update([&](const float dt) { check_collision(dt); });
        }

        void damage_collision::check_collision(const float dt)
        {
            const auto& entities = get_entities();
            std::unordered_map<ecs::entity, std::future<std::set<ecs::entity>>> near_entities;
            std::set<entity> dead_entites_to_remove;

            size_t chunk_size = 100;

            for (size_t i = 0; i < entities.size(); i += chunk_size) {
                std::vector<entity> group;
	            const auto group_end = std::min(i + chunk_size, entities.size());
                std::move(entities.begin(), entities.begin() + chunk_size, std::back_inserter(group));
                find_near_entities(group);
            }

           for (auto& collision_data : futures)
            {
               const auto current_entity = collision_data.first;
               const auto nearby_entities = collision_data.second.get();

               const std::set<entity>& dead_entities = apply_damage(current_entity, nearby_entities);
               std::ranges::move(dead_entities, std::inserter(dead_entites_to_remove, dead_entites_to_remove.begin()));
            }

            for (auto& dead_entity : dead_entites_to_remove)
            {
                world.remove_entity(dead_entity);
            }
        }

        void damage_collision::find_near_entities(const std::vector<entity>& entities)
        {
            for (const auto& entity : entities)
            {
                futures[entity] = std::async(
                    std::launch::async,
                    [](const ecs::entity& entity, ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world)
                    {
                        auto [position, box_collider] = world.get_components<components::position, components::box_collider>(entity);
                        const SDL_FRect& rect_data = ::collision::get_rect_data(position, box_collider);

                        return world.find_nearby_grid<components::damage>(rect_data, { entity });
                    },
                    entity,
                        std::ref(world)
                        );
            }
        }

        std::set<ecs::entity> damage_collision::apply_damage(const ecs::entity entity, const std::set<ecs::entity>& nearby_entities) const
        {
            std::set<ecs::entity> dead_entities;

            for (auto& nearby_entity : nearby_entities)
            {
                if (world.has_component<components::health>(nearby_entity))
                {
                    if (::collision::is_colliding(get_rect_data(entity), get_rect_data(nearby_entity)))
                    {
                        auto [damage] = world.get_components<components::damage>(entity);
                        auto [health] = world.get_components<components::health>(nearby_entity);

                        health.current_health -= damage.damage_to_deal;

                        if (health.current_health <= 0)
                        {
                            dead_entities.insert(nearby_entity);
                        }
                    }
                }
            }

            return dead_entities;
        }
    }
}

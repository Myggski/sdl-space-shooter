#include "pch.h"

#include "damage_collision.h"
#include "ecs/components/position.h"
#include "ecs/components/box_collider.h"
#include "ecs/world.h"
#include "collision/aabb.h"
#include "ecs/components/damage.h"
#include "ecs/components/health.h"
#include "ecs/components/layer.h"
#include "ecs/components/points.h"

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
            set_all_requirements<components::position, components::box_collider, components::damage, components::layer>();
            set_update([&](const float dt) { try_collide(dt); });
        }

        constexpr size_t group_size = 35;
        constexpr size_t min_entities_size = 350;

        void damage_collision::try_collide(const float dt) const
        {
            const auto& entities = get_entities();
            const auto& nearby_entities = search_nearby_entities(entities);
            const auto& dead_entities = check_for_collisions(nearby_entities);

            remove_dead_entities(dead_entities);
        }

        std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>> damage_collision::search_nearby_entities(const std::vector<entity>& entities) const
        {
            std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>> nearby_entities;
            nearby_entities.reserve(ecs::MAX_ENTITIES);

            for (const auto& entity : entities)
            {
                auto [position, box_collider] = world.get_components<components::position, components::box_collider>(entity);
                SDL_FRect rect_data = ::collision::get_rect_data(position, box_collider);

                nearby_entities[entity] = world.find_nearby_grid<components::health>(rect_data, { entity });
            }

            return nearby_entities;
        }

        std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>> damage_collision::group_search(const std::vector<ecs::entity>& entities) const
        {
            std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>> nearby_entities;
            std::vector<entity> group;
            group.reserve(group_size);

            for (size_t i = 0; i < entities.size(); i += group_size) {
                const auto group_end = std::min(i + group_size, entities.size());
                std::move(entities.begin() + i, entities.begin() + group_end, std::back_inserter(group));
                std::ranges::move(search_nearby_entities(group), std::inserter(nearby_entities, nearby_entities.begin()));
                group.clear();
            }

            return nearby_entities;
        }

        std::unordered_set<ecs::entity> damage_collision::check_for_collisions(const std::unordered_map<ecs::entity, std::unordered_set<ecs::entity>>& nearby_entities_data) const
        {
            std::unordered_set<entity> dead_entities;

            for (auto& nearby_data : nearby_entities_data)
            {
                const auto current_entity = nearby_data.first;
                const auto nearby_entities = nearby_data.second;

                std::ranges::move(try_apply_damage(current_entity, nearby_entities), std::inserter(dead_entities, dead_entities.begin()));
            }

            return dead_entities;
        }

        std::unordered_set<ecs::entity> damage_collision::try_apply_damage(const ecs::entity entity, const std::unordered_set<ecs::entity>& nearby_entities) const
        {
            std::unordered_set<ecs::entity> dead_entities;

            for (const auto& nearby_entity : nearby_entities)
            {
                // Checks if nearby has health and a box collider
                if (!world.has_components<components::health, components::box_collider>(nearby_entity))
                {
                    continue;
                }

                auto& collide_layer = world.get_component<components::box_collider>(entity).collides_with;
                const auto nearby_entity_layer = world.has_component<components::layer>(nearby_entity)
                    ? world.get_component<components::layer>(nearby_entity).layer_type
                    : layer_types::none;

                // Checks if it both entities can collide with each other
                if ((collide_layer & components::collide_layer{ static_cast<size_t>(nearby_entity_layer) }) != collide_layer)
                {
                    continue;
                }

                // Checks if the two entities collides
                if (!::collision::is_colliding(get_rect_data(entity), get_rect_data(nearby_entity)))
                {
                    continue;
                }

                // Applying damage
                const auto& damage = world.get_component<components::damage>(entity);
                auto& health = world.get_component<components::health>(nearby_entity);

                health.current_health -= damage.damage_to_deal;

                // Checks if it's dead
                if (health.current_health > 0)
                {
                    continue;
                }

                dead_entities.insert(entity);
                dead_entities.insert(nearby_entity);
            }

            return dead_entities;
        }

        void damage_collision::remove_dead_entities(const std::unordered_set<ecs::entity>& dead_entities) const
        {
            for (auto& dead_entity : dead_entities)
            {
                // Don't look, ugly :(
                if (world.get_component<components::layer>(dead_entity).layer_type == layer_types::enemy)
                {
                    world.add_component<components::points>(dead_entity, components::points(5));
                }

                world.remove_entity(dead_entity);
            }
        }
    }
}

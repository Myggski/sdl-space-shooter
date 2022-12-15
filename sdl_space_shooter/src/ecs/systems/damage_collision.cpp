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
            set_update([&](const float dt) { check_collision(dt); });
            nearby_data_async.reserve(ecs::MAX_ENTITIES);
        }

        constexpr size_t group_size = 35;
        constexpr size_t min_entities_size = 300;

        void damage_collision::check_collision(const float dt)
        {
            const auto& entities = get_entities();

            std::unordered_set<entity> dead_entites_to_remove;

            entities.size() < min_entities_size
        		? search_nearby_entities_async(entities)
        		: group_search(entities);

            for (auto& nearby_data : nearby_data_async)
            {
                nearby_data.second.wait();

                const auto current_entity = nearby_data.first;
                const auto nearby_entities = nearby_data.second.get();

                const std::unordered_set<entity>& dead_entities = try_apply_damage(current_entity, nearby_entities);
                std::ranges::move(dead_entities, std::inserter(dead_entites_to_remove, dead_entites_to_remove.begin()));
            }

            for (auto& dead_entity : dead_entites_to_remove)
            {
                // Don't look, ugly :(
                if (world.get_component<components::layer>(dead_entity).layer_type == layers_types::enemy)
                {
                    world.add_component<components::points>(dead_entity, components::points(5));
                }

                world.remove_entity(dead_entity);
            }
        }

        void damage_collision::group_search(const std::vector<ecs::entity>& entities)
        {
            std::vector<entity> group;
            group.reserve(group_size);


            for (size_t i = 0; i < entities.size(); i += group_size) {
                const auto group_end = std::min(i + group_size, entities.size());
                std::move(entities.begin() + i, entities.begin() + group_end, std::back_inserter(group));
                search_nearby_entities_async(group);

                group.clear();
            }
        }


        void damage_collision::search_nearby_entities_async(const std::vector<entity>& entities)
        {
            for (const auto& entity : entities)
            {
                nearby_data_async[entity] = std::async(
                    std::launch::async,
                    [](const ecs::entity& entity, ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world)
                    {
                        auto [position, box_collider] = world.get_components<components::position, components::box_collider>(entity);
                        const SDL_FRect& rect_data = ::collision::get_rect_data(position, box_collider);

                        return world.find_nearby_grid<components::health>(rect_data, { entity });
                    },
                    entity,
                        std::ref(world)
                        );
            }
        }

        std::unordered_set<ecs::entity> damage_collision::try_apply_damage(const ecs::entity entity, const std::unordered_set<ecs::entity>& nearby_entities) const
        {
            std::unordered_set<ecs::entity> dead_entities;

            for (auto& nearby_entity : nearby_entities)
            {
                if (world.has_component<components::health>(nearby_entity))
                {
                    const auto nearby_entity_layer = world.has_component<components::layer>(nearby_entity)
                        ? world.get_component<components::layer>(nearby_entity).layer_type
                        : layers_types::none;

                    auto& collide_layer = world.get_component<components::box_collider>(entity).collides_with;
                    ecs::components::collide_layer nearby_collide_layer{ static_cast<size_t>(nearby_entity_layer) };


                    if ((collide_layer & nearby_collide_layer) != collide_layer)
                    {
                        continue;
                    }


                    if (::collision::is_colliding(get_rect_data(entity), get_rect_data(nearby_entity)))
                    {
                        auto& damage = world.get_component<components::damage>(entity);
                        auto& health = world.get_component<components::health>(nearby_entity);

                        health.current_health -= damage.damage_to_deal;

                        if (health.current_health <= 0)
                        {
                            dead_entities.insert(entity);
                            dead_entities.insert(nearby_entity);
                        }
                    }
                }
            }

            return dead_entities;
        }

        void damage_collision::on_valid_entity_removed(ecs::entity entity)
        {
            nearby_data_async.erase(entity);
        }
    }
}

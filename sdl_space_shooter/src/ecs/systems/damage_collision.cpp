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

        void damage_collision::check_collision(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                const auto rect_data = get_rect_data(entity);
                const auto nearby_entities = world.find_nearby_grid(rect_data, { entity });

                for (auto& nearby_entity : nearby_entities)
                {
                    if (world.has_component<components::health>(nearby_entity))
                    {
                        if (::collision::is_colliding(rect_data, get_rect_data(nearby_entity)))
                        {
                            auto [damage] = world.get_components<components::damage>(entity);
                            auto [health] = world.get_components<components::health>(nearby_entity);

                            health.current_health -= damage.damage_to_deal;

                            if (health.current_health <= 0)
                            {
                                world.remove_entity(nearby_entity);
                            }
                        }
                    }
                }
            }
        }
    }
}

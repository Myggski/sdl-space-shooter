#include "pch.h"

#include "physics_system.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/position.h"
#include "ecs/components/velocity.h"
#include "ecs/world.h"
#include "collision/aabb.h"
#include "ecs/components/input.h"

namespace ecs
{
    namespace systems
    {
        physics_system::physics_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world) : system(world)
        {
            set_all_requirements<components::position, components::velocity>();
            set_update([&](const float dt) { try_move(dt); });
        }

        void physics_system::try_move(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                auto [position, velocity] = world.get_components<components::position, components::velocity>(entity);
                components::position old_position = position;

                position.x += velocity.x * 100.f * dt;
                position.y += velocity.y * 100.f * dt;
                
                if (world.has_component<components::box_collider>(entity) && (old_position.x - position.x != 0.f || old_position.y - position.y != 0.f))
                {
                    const auto box_collider = world.get_component<components::box_collider>(entity);
                    const auto new_rect_data = collision::get_rect_data(position, box_collider);

                    world.update_grid(entity, new_rect_data);
                }
            }
        }
    }
}

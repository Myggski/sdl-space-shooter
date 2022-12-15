#include "pch.h"

#include "physics_system.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/position.h"
#include "ecs/components/velocity.h"
#include "ecs/world.h"
#include "collision/aabb.h"
#include "ecs/components/input.h"
#include "ecs/components/texture.h"
#include "application/math.h"
#include "ecs/components/rotation.h"

namespace ecs
{
    namespace systems
    {
        constexpr float movement_tolerance = 0.001f;

        physics_system::physics_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world) : system(world)
        {
            set_all_requirements<components::position, components::velocity, components::box_collider>();
            set_update([&](const float dt) { try_update_position(dt); });
        }

        void physics_system::try_update_position(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                auto [position, velocity, box_collider] = world.get_components<components::position, components::velocity, components::box_collider>(entity);

                position.old_x = position.x;
                position.old_y = position.y;
                position.x += velocity.x * 100.f * dt;
                position.y += velocity.y * 100.f * dt;

                if (world.has_component<components::input>(entity))
                {
                    const auto [texture, rotation] = world.get_components<components::texture, components::rotation>(entity);
                    
	                if (position.x <= 0 || position.x + texture.width >= world.get_width())
	                {
                        position.x = position.old_x;
	                }

                    if (position.y <= 0 || position.y + texture.height >= world.get_height())
                    {
                        position.y = position.old_y;
                    }
                }

                // Checks if position has changed
                if (std::abs(position.old_x - position.x) > movement_tolerance || std::abs(position.old_y - position.y) > movement_tolerance)
                {
                    world.update_grid(entity, collision::get_rect_data(position, box_collider));
                }
            }
        }
    }
}

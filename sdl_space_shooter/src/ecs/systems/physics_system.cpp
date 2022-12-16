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

        void physics_system::try_update_position(const float dt)
        {
            for (const auto& entity : get_entities())
            {
                auto [position, velocity, box_collider] = world.get_components<components::position, components::velocity, components::box_collider>(entity);

                const SDL_FPoint old_position{
                    position.x,
                    position.y
                };
                SDL_FPoint new_position{
                    position.x + velocity.x * 100.f * dt,
                    position.y + velocity.y * 100.f * dt
                };

                if (world.has_component<components::input>(entity))
                {
                    player_screen_wrap(entity, velocity, new_position, old_position);
                }

                // Checks if position has changed
                if (std::abs(old_position.x - new_position.x) > movement_tolerance || std::abs(old_position.y - new_position.y) > movement_tolerance)
                {
                    position.old_x = position.x;
                    position.old_y = position.y;
                    position.x = new_position.x;
                    position.y = new_position.y;
                    world.update_grid(entity, collision::get_rect_data(position, box_collider));
                }
            }
        }

        void physics_system::player_screen_wrap(ecs::entity entity, ecs::components::velocity& velocity, SDL_FPoint& new_position, const SDL_FPoint& old_position)
        {
            const auto [texture, rotation] = world.get_components<components::texture, components::rotation>(entity);

            const SDL_FRect image_rect = {
                round(new_position.x),
                round(new_position.y),
                texture.width,
                texture.height,
            };
            const SDL_FRect bounded_rotation_rect = math::get_bound_rect(image_rect, rotation.angle);
            const float rotation_diff_x = abs(round(new_position.x) - round(bounded_rotation_rect.x));
            const float rotation_diff_y = abs(round(new_position.y) - round(bounded_rotation_rect.y));
            SDL_FPoint out_of_bound_direction = { 0.f, 0.f };

            if (bounded_rotation_rect.x <= 0.f)
            {
                out_of_bound_direction.x -= 1.f;
            }
            else if (bounded_rotation_rect.x + bounded_rotation_rect.w >= world.get_width())
            {
                out_of_bound_direction.x += 1.f;
            }
            else if (bounded_rotation_rect.y <= 0.f)
            {
                out_of_bound_direction.y -= 1.f;
            }
            else if (bounded_rotation_rect.y + bounded_rotation_rect.h >= world.get_height())
            {
                out_of_bound_direction.y += 1.f;
            }

            if (out_of_bound_direction.x != 0.f)
            {
                new_position.x = round(out_of_bound_direction.x < 0.f ? -rotation_diff_x : world.get_width() - bounded_rotation_rect.w - rotation_diff_x);
                velocity.x = -math::dot(velocity.x, velocity.y, out_of_bound_direction.x, out_of_bound_direction.y) * out_of_bound_direction.x;

                if (abs(new_position.x - old_position.x) <= 0.9f)
                {
                    new_position.x = old_position.x;
                }
            }
            else if (out_of_bound_direction.y != 0.f)
            {
                new_position.y = round(out_of_bound_direction.y < 0.f ? rotation_diff_y : world.get_height() - bounded_rotation_rect.h + rotation_diff_y);
                velocity.y = -math::dot(velocity.x, velocity.y, out_of_bound_direction.x, out_of_bound_direction.y) * out_of_bound_direction.y;

                if (abs(new_position.y - old_position.y) <= 0.9f)
                {
                    new_position.y = old_position.y;
                }
            }
        }
    }
}

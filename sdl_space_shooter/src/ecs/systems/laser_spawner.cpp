#include "pch.h"

#include "laser_spawner.h"
#include "ecs/components/input.h"
#include "ecs/world.h"
#include "ecs/entities/laser.h"

namespace ecs
{
    namespace systems
    {
        constexpr float laser_delay = 0.06f;
        static bool shooting_zero = false;

        laser_spawner::laser_spawner(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::texture_manager& texture_manager)
            : system(world), texture_manager(texture_manager), delay(laser_delay)
        {
            set_all_requirements<components::input>();
            set_update([&](const float dt)
            {
	            spawn_laser(dt);
            });
        }

        void laser_spawner::spawn_laser(const float dt)
        {
            for (const auto& entity : get_entities())
            {
                const auto [input, position, texture, velocity] = world.get_components<components::input, components::position, components::texture, components::velocity>(entity);

                if (delay >= laser_delay && input.is_firing)
                {
                    components::position laser_position = components::position(position.x + (texture.width / 2.f), position.y + (texture.height / 2.f) - (55.f / 2.f));
                    entities::create_laser(world, texture_manager, laser_position, shooting_zero);

                    shooting_zero = !shooting_zero;
                    delay = 0;
                }

                delay += dt;
            }
        }
    }
}



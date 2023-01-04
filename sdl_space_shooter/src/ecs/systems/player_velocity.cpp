#include "pch.h"

#include "player_velocity.h"

#include "ecs/components/input.h"
#include "ecs/components/velocity.h"
#include "ecs/world.h"

namespace ecs
{
    namespace systems
    {
        constexpr float SPEED = 15;
        constexpr float FRICTION = 2.5;

        player_velocity::player_velocity(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world)
            : system(world)
        {
            set_all_requirements<components::input, components::velocity>();
            set_update([&](const float dt) { update_velocity(dt); });
        }

        void player_velocity::update_velocity(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                auto [input, velocity] = world.get_components<components::input, components::velocity>(entity);

                velocity.x = velocity.x + ((input.x * SPEED) - (velocity.x * FRICTION)) * dt;
                velocity.y = velocity.y + ((input.y * SPEED) - (velocity.y * FRICTION)) * dt;
            }
        }
    }
}



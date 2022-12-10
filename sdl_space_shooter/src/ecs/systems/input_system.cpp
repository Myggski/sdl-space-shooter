#include "pch.h"

#include "input_system.h"
#include "ecs/components/input.h"
#include "ecs/components/velocity.h"
#include "ecs/world.h"
#include "helpers/math.h"

namespace ecs
{
    namespace systems
    {
        input_system::input_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::keyboard& keyboard)
            : system(world), keyboard(keyboard)
        {
            set_requirements<components::input, components::velocity>();
        }
        void input_system::update(const float dt) const
        {

            for (const auto& entity : get_entities())
            {
                float new_velocity_x = 0;
                float new_velocity_y = 0;

                auto [input, velocity] = world.get_components<components::input, components::velocity>(entity);

                if (keyboard.is_key_pressed(input.move_up))
                {
                    new_velocity_y -= 1.f;
                }

                if (keyboard.is_key_pressed(input.move_down))
                {
                    new_velocity_y += 1.f;
                }

                if (keyboard.is_key_pressed(input.move_left))
                {
                    new_velocity_x -= 1.f;
                }

                if (keyboard.is_key_pressed(input.move_right))
                {
                    new_velocity_x += 1.f;
                }

                const float magnitude = math::magnitude(new_velocity_x, new_velocity_y);
                velocity.x = new_velocity_x / magnitude;
                velocity.y = new_velocity_y / magnitude;
            }
        }
    }
}
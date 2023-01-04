#include "pch.h"

#include "player_input.h"

#include "application/keyboard.h"
#include "ecs/components/input.h"
#include "ecs/world.h"
#include "utils/math.h"

namespace ecs
{
    namespace systems
    {
        player_input::player_input(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::keyboard& keyboard)
            : system(world), keyboard(keyboard)
        {
            set_all_requirements<components::input>();
            set_update([&](const float dt) { check_input(dt); });
        }
        void player_input::check_input(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                float input_x = 0;
                float input_y = 0;

                auto& input = world.get_component<components::input>(entity);

                if (keyboard.is_key_pressed(input.move_up))
                {
                    input_y -= 1.f;
                }

                if (keyboard.is_key_pressed(input.move_down))
                {
                    input_y += 1.f;
                }

                if (keyboard.is_key_pressed(input.move_left))
                {
                    input_x -= 1.f;
                }

                if (keyboard.is_key_pressed(input.move_right))
                {
                    input_x += 1.f;
                }

                const float magnitude = utils::math::magnitude(input_x, input_y);
                input.x = input_x / magnitude;
                input.y = input_y / magnitude;

                input.is_firing = keyboard.is_key_pressed(input.fire_laser);
            }
        }
    }
}
#include "pch.h"

#include "input_system.h"
#include "components/input.h"
#include "components/velocity.h"
#include "ecs/world.h"

namespace ecs
{
    input_system::input_system(ecs::world<ecs::ComponentCount, ecs::SystemCount>& world, application::keyboard& keyboard)
	: world(world), keyboard(keyboard)
    {
        set_requirements<components::input, components::velocity>();
    }
    void input_system::update(const float dt) const
    {
        
        for (const auto& entity : get_managed_entities())
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
            
            velocity.x = new_velocity_x;
            velocity.y = new_velocity_y;
        }
    }
}

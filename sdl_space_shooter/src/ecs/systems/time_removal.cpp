#include "pch.h"

#include "time_removal.h"
#include "ecs/components/removal_timer.h"
#include "ecs/entities/laser.h"


namespace ecs
{
    namespace systems
    {
        constexpr float SPEED = 30;
        constexpr float FRICTION = 3.5;

        time_removal::time_removal(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world)
            : system(world)
        {
            set_all_requirements<components::removal_timer>();
            set_update([&](const float dt) { try_destroy(dt); });
        }

        void time_removal::try_destroy(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                if (world.is_alive(entity))
                {
                    auto& removal_timer = world.get_component<components::removal_timer>(entity);

                    removal_timer.current_time -= dt;

                    if (removal_timer.current_time <= 0)
                    {
                        world.remove_entity(entity);
                    }
                }
            }
        }
    }
}



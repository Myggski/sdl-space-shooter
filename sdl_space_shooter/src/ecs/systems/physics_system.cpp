#include "pch.h"

#include "physics_system.h"
#include "ecs/components/position.h"
#include "ecs/components/velocity.h"
#include "ecs/world.h"

namespace ecs
{
    namespace systems
    {
        physics_system::physics_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world) : system(world)
        {
            set_requirements<components::position, components::velocity>();
        }
        void physics_system::update(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                auto [position, velocity] = world.get_components<components::position, components::velocity>(entity);
                position.x += velocity.x * 100.f * dt;
                position.y += velocity.y * 100.f * dt;
            }
        }
    }
}
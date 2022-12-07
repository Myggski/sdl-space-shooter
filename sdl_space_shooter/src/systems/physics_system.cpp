#include "pch.h"

#include "physics_system.h"
#include "components/position.h"
#include "components/velocity.h"
#include "ecs/world.h"

namespace ecs
{
    physics_system::physics_system(ecs::world<ecs::ComponentCount, ecs::SystemCount>& world) : world(world)
    {
        set_requirements<components::position, components::velocity>();
    }
    void physics_system::update(const float dt) const
    {
        for (const auto& entity : get_managed_entities())
        {
            auto [position, velocity] = world.get_components<components::position, components::velocity>(entity);
            position.x += velocity.x * 100.f * dt;
            position.y += velocity.y * 100.f * dt;
        }
    }
}

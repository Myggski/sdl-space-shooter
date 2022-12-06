#pragma once

#include "ecs/system.h"
#include "ecs/world.h"

namespace ecs
{
    class physics_system : public ecs::system<ecs::ComponentCount, ecs::SystemCount>
    {
    public:
	    physics_system(ecs::world<ecs::ComponentCount, ecs::SystemCount>& world);
        void update(const float dt) const;
    private:
        ecs::world<ComponentCount, SystemCount>& world;
    };
}


#pragma once

#include "application/keyboard.h"
#include "ecs/system.h"
#include "ecs/world.h";

namespace ecs
{
    class input_system final : public ecs::system<ecs::ComponentCount, ecs::SystemCount>
    {
    public:
        input_system(ecs::world<ecs::ComponentCount, ecs::SystemCount>& world, application::keyboard& keyboard);
        void update(const float dt) const override;
    private:
        ecs::world<ComponentCount, SystemCount>& world;
        application::keyboard& keyboard;
    };
}


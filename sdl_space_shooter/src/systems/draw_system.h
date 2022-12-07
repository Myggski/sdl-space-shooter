#pragma once

#include "ecs/system.h"
#include "ecs/world.h"
#include <SDL.h>

namespace ecs
{
    class draw_system final : public ecs::system<ecs::ComponentCount, ecs::SystemCount>
    {
    public:
        draw_system(ecs::world<ecs::ComponentCount, ecs::SystemCount>& world, SDL_Renderer* renderer);
        void update(const float dt) const override;
    private:
        ecs::world<ComponentCount, SystemCount>& world;
        SDL_Renderer* renderer;
    };
}


#pragma once

#include "ecs/system.h"
#include "ecs/world.h"
#include <SDL.h>

namespace ecs
{
    namespace systems
    {
        class draw_system final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            draw_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, SDL_Renderer* renderer);
            void update(const float dt) const override;
        private:
            SDL_Renderer* renderer;
        };
    }
}
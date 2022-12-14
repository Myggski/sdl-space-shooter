#pragma once

#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace systems
    {
        class draw_system final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            draw_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, SDL_Renderer* renderer);
            void draw(const float dt);
            void on_valid_entity_added(entity entity) override;
            void on_valid_entity_removed(entity entity) override;
        private:
            std::vector<entity> draw_entities;
            SDL_Renderer* renderer;
            bool is_dirty;
        };
    }
}

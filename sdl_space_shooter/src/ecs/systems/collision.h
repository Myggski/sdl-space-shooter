﻿#pragma once

#include "ecs/system.h"
#include "collision/spatial_grid.h"
#include "ecs/world.h"

namespace ecs
{
    namespace systems
    {
        class collision final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            collision(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world);
            void on_valid_entity_added(entity entity) override;
            void on_valid_entity_removed(entity entity) override;
        private:
            const SDL_FRect get_rect_data(entity entity) const;
        };
    }
}

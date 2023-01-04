#pragma once

#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace components
    {
        struct text;
    }

    namespace systems
    {
        class scoreboard final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            scoreboard(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, entity score_text_entity);

        private:
            void on_valid_entity_added(entity entity) override;

        private:
            components::text& score_text_component;
            int current_score;
        };
    }
}

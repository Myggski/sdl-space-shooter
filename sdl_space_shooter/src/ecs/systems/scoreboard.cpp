#include "pch.h"

#include "scoreboard.h"
#include "ecs/system.h"
#include "ecs/setup_data.h"
#include "ecs/world.h"
#include "ecs/components/points.h"

namespace ecs
{
    namespace systems
    {
        constexpr float SPEED = 30;
        constexpr float FRICTION = 3.5;

        scoreboard::scoreboard(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, ecs::entity score_text_entity)
            : system(world), score_text_component(world.get_component<components::text>(score_text_entity))
        {
            set_all_requirements<components::points>();
        }

        void scoreboard::on_valid_entity_added(entity entity)
        {
            current_score += world.get_component<components::points>(entity).total_points;

            char buffer[16];
            std::snprintf(buffer, sizeof(buffer), "Score: %d", current_score);
            score_text_component.text_string.clear();
            score_text_component.text_string.append(std::string(buffer));
        }
    }
}



﻿#pragma once

#include "application/interval.h"
#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace application
{
    class texture_manager;

}

namespace ecs
{
    namespace systems
    {
        class enemy_spawner final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            enemy_spawner(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::texture_manager& texture_manager);
            ~enemy_spawner() override;
        private:
            void spawn_enemy(const float dt);
            std::unordered_map<std::string, std::pair<int, application::interval_function>> intervals;
            application::texture_manager& texture_manager;
            float current_timer;
            float next_spawn_time;
            float max_spawn_timer;
            float min_spawn_timer;
            int number_of_enemies_to_spawn;
            application::interval_function increase_spawning_interval;
            application::interval_function increase_spawning_enemies;
        };
    }
}

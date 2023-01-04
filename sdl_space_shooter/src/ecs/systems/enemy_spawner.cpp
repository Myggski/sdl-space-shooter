#include "pch.h"

#include "enemy_spawner.h"

#include "utils/interval.h"
#include "utils/random.h"
#include "ecs/components/position.h"
#include "ecs/components/velocity.h"
#include "ecs/world.h"
#include "ecs/entities/enemy.h"

namespace ecs
{
    namespace systems
    {
        constexpr int64_t SPAWNING_TIMER_INTERVAL = 15;
        constexpr int64_t SPAWNING_NUMBER_INTERVAL = 30;
        constexpr float MAX_SPAWN_TIMER_START = 1.5f;
        constexpr float MIN_SPAWN_TIMER = 0.2f;

        enemy_spawner::enemy_spawner(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::texture_manager& texture_manager)
            : system(world), texture_manager(texture_manager), current_timer(MAX_SPAWN_TIMER_START), next_spawn_time(MAX_SPAWN_TIMER_START),
            max_spawn_timer(MAX_SPAWN_TIMER_START), min_spawn_timer(MIN_SPAWN_TIMER), number_of_enemies_to_spawn(0)
        {
            set_all_requirements<components::position, components::velocity>();
            set_update([&](const float dt) { spawn_enemy(dt); });

            increase_spawning_interval = [&]() { max_spawn_timer -= 0.4f; };
            increase_spawning_enemies = [&]() { number_of_enemies_to_spawn += 1; };

            utils::interval::get_instance().insert(SPAWNING_TIMER_INTERVAL, &increase_spawning_interval);
            utils::interval::get_instance().insert(SPAWNING_NUMBER_INTERVAL, &increase_spawning_enemies);
        }

        enemy_spawner::~enemy_spawner()
        {
	        utils::interval::get_instance().remove(&increase_spawning_interval);
	        utils::interval::get_instance().remove(&increase_spawning_enemies);
        }

        void enemy_spawner::spawn_enemy(const float dt)
        {
            if (current_timer >= next_spawn_time)
            {
                for (int i = 0; i < number_of_enemies_to_spawn; i++)
                {

                    entities::create_enemy(world, texture_manager, utils::get_random<float>(0.f, 636.f));
                }

                next_spawn_time = utils::get_random(std::max(max_spawn_timer * 0.6f, min_spawn_timer), std::max(max_spawn_timer * 1.2f, min_spawn_timer));
                current_timer = 0.f;

            }

            current_timer += dt;
        }
    }
}

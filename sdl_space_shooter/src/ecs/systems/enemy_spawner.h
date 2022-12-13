#pragma once

#include <random>

#include "application/interval.h"
#include "application/texture_manager.h"
#include "ecs/system.h"
#include "ecs/world.h"

namespace ecs
{
    namespace systems
    {

        template<class T>
        using uniform_distribution =
            typename std::conditional<
            std::is_floating_point<T>::value,
            std::uniform_real_distribution<T>,
            typename std::conditional<
            std::is_integral<T>::value,
            std::uniform_int_distribution<T>,
            void
            >::type
            >::type;

        class enemy_spawner final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            enemy_spawner(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::texture_manager& texture_manager);
            ~enemy_spawner() override;
            void spawn_enemy(const float dt);
        private:
            std::unordered_map<std::string, std::pair<int, std::function<void()>>> intervals;
            application::texture_manager& texture_manager;
            float current_timer;
            float next_spawn_time;
            float max_spawn_timer;
            float min_spawn_timer;
            int number_of_enemies_to_spawn;
            application::interval_function increase_spawning_interval;
            application::interval_function increase_spawning_enemies;

            template <class T>
            T get_random(T min, T max) {
                std::random_device rd;
                std::ranlux48 gen(rd());
                const uniform_distribution<T> dis(min, max);
                return dis(gen);
            }
        };
    }
}

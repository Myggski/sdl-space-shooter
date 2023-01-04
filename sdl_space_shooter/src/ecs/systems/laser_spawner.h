#pragma once

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
        class laser_spawner final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            laser_spawner(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::texture_manager& texture_manager);

        private:
            void spawn_laser(const float dt);

        private:
            application::texture_manager& texture_manager;
            float delay;
        };
    }
}

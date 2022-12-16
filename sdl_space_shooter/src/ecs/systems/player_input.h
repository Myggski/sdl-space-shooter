#pragma once

#include "application/keyboard.h"
#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace systems
    {
        class player_input final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            player_input(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::keyboard& keyboard);
        private:
            void check_input(const float dt) const;
            application::keyboard& keyboard;
        };
    }
}

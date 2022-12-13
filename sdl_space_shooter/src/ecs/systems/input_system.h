#pragma once

#include "application/keyboard.h"
#include "ecs/system.h"
#include "ecs/world.h";

namespace ecs
{
    namespace systems
    {
        class input_system final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            input_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::keyboard& keyboard);
            void check_input(const float dt) const;
        private:
            application::keyboard& keyboard;
        };
    }
}

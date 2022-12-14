#pragma once

#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    namespace systems
    {
        class time_removal final : public system<MAX_COMPONENTS, MAX_SYSTEMS>
        {
        public:
            time_removal(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world);
        private:
            void try_destroy(const float dt) const;
        };
    }
}

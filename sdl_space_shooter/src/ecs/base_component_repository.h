#pragma once

#include "entity.h"

namespace ecs
{
    class base_component_repository
    {
    public:
        virtual ~base_component_repository() = default;

        virtual void reserve(std::size_t size) = 0;
        virtual bool try_remove(ecs::entity entity) = 0;
    };

}

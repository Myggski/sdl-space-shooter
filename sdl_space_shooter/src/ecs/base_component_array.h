#pragma once

#include "ecs/entity.h"

namespace ecs
{
    class base_component_array
    {
    public:
        virtual ~base_component_array() = default;

        virtual void reserve(size_t size) = 0;
        virtual void clear() = 0;
        virtual bool try_remove(entity entity) = 0;
    };
}

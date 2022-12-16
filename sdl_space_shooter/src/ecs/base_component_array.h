#pragma once

#include "ecs/entity.h"

namespace ecs
{
    /**
     * \brief This class only exist in order to have components in lists/arrays
     */
    class base_component_array
    {
    public:
        virtual ~base_component_array() = default;

        virtual void reserve(size_t size) = 0;
        virtual bool try_remove(entity entity) = 0;
    };
}

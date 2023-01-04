#pragma once

#include "spatial_data_structure.h"

namespace collision
{
    enum class spatial_structure_types
    {
	    GRID,
        QUADTREE,
    };

    template<typename T>
    class spatial_data_structure_factory
    {
    public:
        static std::unique_ptr<spatial_data_structure<T>> create(spatial_structure_types type, int width, int height, int max_entities)
        {
            switch (type)
            {
            case spatial_structure_types::GRID:
                return std::make_unique<collision::spatial_grid<T>>(max_entities, width, height);
            case spatial_structure_types::QUADTREE:
                return std::make_unique<collision::quad_tree<T>>(SDL_FRect{ 0.f, 0.f, static_cast<float>(width), static_cast<float>(height) }, max_entities);
            default:
                throw std::invalid_argument("Invalid spatial structure type");
            }
        }
    };
}

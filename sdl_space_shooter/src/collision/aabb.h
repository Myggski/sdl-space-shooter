#pragma once

#include "SDL.h"
#include "ecs/components/position.h"
#include "ecs/components/box_collider.h"

namespace collision
{
	inline bool is_colliding(const SDL_FRect& a, const SDL_FRect& b)
    {
        return (
            a.x <= b.x + b.w 
            && a.x + a.w >= b.x 
            && a.y <= b.y + b.h 
            && a.y + a.h >= b.y
        );
    }

    inline SDL_FRect get_rect_data(const ecs::components::position& position, const ecs::components::box_collider& box_collider)
    {
        const float x = position.x + box_collider.offset_x;
        const float y = position.y + box_collider.offset_y;

        return SDL_FRect{ x, y, box_collider.width, box_collider.height };
    }
}

#pragma once
#include <pch.h>

#include <random>
#include <type_traits>

namespace math
{
    constexpr float TAU = 6.2831853071;
	inline float magnitude(const float x, const float y)
	{
		const auto magnitude = pow(x * x + y * y, 0.5f);
		return magnitude == 0.f ? 1.f : magnitude;
	}

    inline const SDL_FRect& rotate_rect(const SDL_FRect& original_rect, float degrees)
    {
        float radians = (degrees * TAU) / 360.f;
        float center_x = original_rect.x + (original_rect.w * 0.5f);
        float center_y = original_rect.y + (original_rect.h * 0.5f);

        float min_x = center_x - (original_rect.w * 0.5f) * cos(radians) - (original_rect.h * 0.5f) * sin(radians);
        float max_x = center_x + (original_rect.w * 0.5f) * cos(radians) + (original_rect.h * 0.5f) * sin(radians);
        float width = max_x - min_x;

        float min_y = center_y - (original_rect.w * 0.5f) * sin(radians) + (original_rect.h * 0.5f) * cos(radians);
        float max_y = center_y + (original_rect.w * 0.5f) * sin(radians) - (original_rect.h * 0.5f) * cos(radians);
        float height = max_y - min_y;

        return SDL_FRect{
            min_x,
            min_y,
            width,
            height,
        };
    }
}

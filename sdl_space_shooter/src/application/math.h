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

    inline const float degrees_to_radians(float degrees)
    {
        return (degrees * TAU) / 360.f;
    }

    inline const float dot(float x1, float y1, float x2, float y2)
    {
        return x1 * x2 + y1 * y2;
    }

    inline SDL_FRect get_bound_rect(const SDL_FRect& rect_to_rotate, float degrees)
    {
        const SDL_Point center = {
            rect_to_rotate.x + rect_to_rotate.w / 2,
            rect_to_rotate.y + rect_to_rotate.h / 2
        };

        float min_x = std::numeric_limits<float>::max();
        float min_y = std::numeric_limits<float>::max();
        float max_x = std::numeric_limits<float>::min();
        float max_y = std::numeric_limits<float>::min();

        for (int i = 0; i < 4; i++) {
            SDL_FPoint corner;

            if (i == 0) {
                corner = { rect_to_rotate.x, rect_to_rotate.y };
            }
            else if (i == 1) {
                corner = { rect_to_rotate.x + rect_to_rotate.w, rect_to_rotate.y };
            }
            else if (i == 2) {
                corner = { rect_to_rotate.x, rect_to_rotate.y + rect_to_rotate.h };
            }
            else {
                corner = { rect_to_rotate.x + rect_to_rotate.w, rect_to_rotate.y + rect_to_rotate.h };
            }

            const float angle = degrees_to_radians(degrees);

            const float s = sin(angle);
            const float c = cos(angle);

            // Translates the position
            double x1 = corner.x - center.x;
            double y1 = corner.y - center.y;

            // Rotating the point
            double x2 = x1 * c - y1 * s;
            double y2 = x1 * s + y1 * c;

            // Translating it back to original position
            corner.x = x2 + center.x;
            corner.y = y2 + center.y;

            min_x = std::min(min_x, corner.x);
            min_y = std::min(min_y, corner.y);
            max_x = std::max(max_x, corner.x);
            max_y = std::max(max_y, corner.y);
        }

        return SDL_FRect{
            min_x,
            min_y,
            max_x - min_x,
            max_y - min_y
        };
    }

}

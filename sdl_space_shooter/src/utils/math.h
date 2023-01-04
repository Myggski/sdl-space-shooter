#pragma once
#include <pch.h>

#include <random>

namespace utils
{
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

        inline SDL_FPoint rotate_and_translate_point(SDL_FPoint point, const SDL_FPoint& center, float s, float c)
        {
            // Translates the position
            const float x1 = point.x - center.x;
            const float y1 = point.y - center.y;

            // Rotating the point
            const float x2 = x1 * c - y1 * s;
            const float y2 = x1 * s + y1 * c;

            // Translating it back to original position
            point.x = x2 + center.x;
            point.y = y2 + center.y;

            return point;
        }

        inline SDL_FRect get_bound_rect(const SDL_FRect& rect_to_rotate, float degrees)
        {
            const SDL_FPoint center = {
                rect_to_rotate.x + rect_to_rotate.w / 2.f,
                rect_to_rotate.y + rect_to_rotate.h / 2.f
            };

            float min_x = std::numeric_limits<float>::max();
            float min_y = std::numeric_limits<float>::max();
            float max_x = std::numeric_limits<float>::min();
            float max_y = std::numeric_limits<float>::min();

            const float angle = degrees_to_radians(degrees);
            const float s = sin(angle);
            const float c = cos(angle);

            SDL_FPoint corners[4] = {
                { rect_to_rotate.x, rect_to_rotate.y }, // top-left
                { rect_to_rotate.x + rect_to_rotate.w, rect_to_rotate.y }, // top-right
                { rect_to_rotate.x, rect_to_rotate.y + rect_to_rotate.h }, // bottom-left
                { rect_to_rotate.x + rect_to_rotate.w, rect_to_rotate.y + rect_to_rotate.h } // bottom-right
            };

            for (const auto& corner : corners)
            {
                SDL_FPoint rotated_corner = rotate_and_translate_point(corner, center, s, c);
                min_x = std::min(min_x, rotated_corner.x);
                min_y = std::min(min_y, rotated_corner.y);
                max_x = std::max(max_x, rotated_corner.x);
                max_y = std::max(max_y, rotated_corner.y);
            }

            return { min_x, min_y, max_x - min_x, max_y - min_y };
        }
    }
}

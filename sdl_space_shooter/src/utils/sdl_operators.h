#pragma once

#include <SDL_pixels.h>

inline bool operator==(const SDL_Color& lhs, const SDL_Color& rhs) {
	return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

inline bool operator==(const SDL_FPoint& lhs, const SDL_FPoint& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator==(const SDL_Point& lhs, const SDL_Point& rhs) {
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

namespace utils
{
	struct SDL_Color_Hash {
		size_t operator()(const SDL_Color& color) const {
			size_t h = 0;

			h = h * 42 + color.r;
			h = h * 42 + color.g;
			h = h * 42 + color.b;

			return h;
		}
	};
}

#pragma once

#include <string>
#include "SDL_rect.h"

namespace utils
{
	inline size_t get_key(int x, int y)
	{
		char buffer[15];
		std::snprintf(buffer, sizeof(buffer), "%d,%d", x, y);
		std::hash<std::string> hasher;
		auto hashed = hasher(buffer);
		return hashed;
	}

	inline size_t get_key(SDL_Point point)
	{
		return get_key(point.x, point.y);
	}
}


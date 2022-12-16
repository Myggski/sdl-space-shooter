#include "pch.h"
#include "position.h"

namespace ecs
{
	namespace components
	{
		position::position()
			: x(std::numeric_limits<float>::max()), y(std::numeric_limits<float>::max()), old_x(std::numeric_limits<float>::max()), old_y(std::numeric_limits<float>::max()) { }
		position::position(float x, float y)
			: x(x), y(y), old_x(x), old_y(y) { }
	}
}

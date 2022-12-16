#include "pch.h"

#include "points.h"

#include "damage.h"

namespace ecs
{
	namespace components
	{
		points::points()
			: points(0) {}
		points::points(int total_points)
			: total_points(total_points) {}
	}
}

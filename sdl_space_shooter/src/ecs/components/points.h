#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct points : component<points, component_types::points>
		{
		public:
			points();
			points(int total_points);

		public:
			int total_points;
		};
	}
}

#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct removal_timer : component<removal_timer, component_types::removal_timer>
		{
		public:
			removal_timer();
			removal_timer(float current_time_seconds);

			float current_time;
		};
	}
}

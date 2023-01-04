#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct position : component<position, component_types::position>
		{
		public:
			position();
			position(float x, float y);

		public:
			float x;
			float y;
			float old_x;
			float old_y;
		};
	}
}

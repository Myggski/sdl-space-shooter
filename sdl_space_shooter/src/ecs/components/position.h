#pragma once

#include "ecs/component.h"

namespace ecs
{
	namespace components
	{
		struct position : component<position, component_types::position>
		{
		public:
			position();
			position(float x, float y);

			float x;
			float y;
		};
	}
}
#pragma once

#include "ecs/component.h"

namespace ecs
{
	namespace components
	{
		struct velocity : component<velocity, component_types::velocity>
		{
		public:
			velocity();
			velocity(float x, float y);

			float x;
			float y;
		};
	}
}
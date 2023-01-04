#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct velocity : component<velocity, component_types::velocity>
		{
		public:
			velocity();
			velocity(float x, float y);

		public:
			float x;
			float y;
		};
	}
}

#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct rotation : component<rotation, component_types::rotation>
		{
		public:
			rotation();
			rotation(double angle);

			double angle;
		};
	}
}

#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct health : component<health, component_types::health>
		{
		public:
			health();
			health(int max_health);

		public:
			int max_health;
			int current_health;
		};
	}
}

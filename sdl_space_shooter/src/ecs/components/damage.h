#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct damage : component<damage, component_types::damage>
		{
		public:
			damage();
			damage(int damage_to_deal);

		public:
			int damage_to_deal;
		};
	}
}

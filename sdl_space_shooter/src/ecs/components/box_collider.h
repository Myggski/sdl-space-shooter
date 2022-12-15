#pragma once

#include <bitset>

#include "component.h"

namespace ecs
{
	namespace components
	{
		using collide_layer = std::bitset<3>;

		struct box_collider : component<box_collider, component_types::box_collider>
		{
		public:
			box_collider();
			box_collider(float width, float height, float offset_x = 0, float offset_y = 0, collide_layer collides_with = { 0 });

			float width;
			float height;
			float offset_x;
			float offset_y;
			collide_layer collides_with;
		};
	}
}

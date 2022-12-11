#pragma once
#include "ecs/component.h"

namespace ecs
{
	namespace components
	{
		struct box_collider : component<box_collider, component_types::box_collider>
		{
		public:
			box_collider();
			box_collider(float width, float height, float offset_x = 0, float offset_y = 0);

			float width;
			float height;
			float offset_x;
			float offset_y;
		};
	}
}

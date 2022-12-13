#include "pch.h"

#include "box_collider.h"

namespace ecs
{
	namespace components
	{
		box_collider::box_collider()
			: width(0), height(0), offset_x(0), offset_y(0) {}
		box_collider::box_collider(float width, float height, float offset_x, const float offset_y)
			: width(width), height(height), offset_x(offset_x), offset_y(offset_y) {}
	}
}

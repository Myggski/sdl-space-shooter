#pragma once

#include "components.h"
#include "ecs/component.h"

namespace components
{
	struct position : ecs::component<position, component_type::position>
	{
	public:
		position();
		position(float x, float y);

		float x;
		float y;
	};
}
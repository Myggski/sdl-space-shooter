#pragma once

#include "components.h"
#include "ecs/component.h"

namespace components
{
	struct velocity : ecs::component<velocity, component_type::velocity>
	{
	public:
		velocity();
		velocity(float x, float y);

		float x;
		float y;
	};
}
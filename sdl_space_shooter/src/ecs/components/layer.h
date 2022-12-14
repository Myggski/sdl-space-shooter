#pragma once

#include "component.h"
#include "ecs/layers_types.h"

namespace ecs
{
	namespace components
	{
		struct layer : component<layer, component_types::layer>
		{
		public:
			layer();
			layer(ecs::layers_types layer_type);

			ecs::layers_types layer_type;
		};
	}
}

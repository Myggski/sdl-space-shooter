#pragma once

#include "component.h"
#include "ecs/layer_types.h"

namespace ecs
{
	namespace components
	{
		struct layer : component<layer, component_types::layer>
		{
		public:
			layer();
			layer(layer_types layer_type);

		public:
			layer_types layer_type;
			size_t z_index;
		};
	}
}

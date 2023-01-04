#include "pch.h"
#include "layer.h"

namespace ecs
{
	namespace components
	{
		layer::layer()
			: layer_type(layer_types::none), z_index(0) { }
		layer::layer(ecs::layer_types layer_type)
			: layer_type(layer_type), z_index(0) { }
	}
}

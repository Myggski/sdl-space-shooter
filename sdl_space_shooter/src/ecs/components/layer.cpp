#include "pch.h"
#include "layer.h"

namespace ecs
{
	namespace components
	{
		layer::layer()
			: layer_type(layers_types::none), z_index(0) { }
		layer::layer(ecs::layers_types layer_type)
			: layer_type(layer_type), z_index(0) { }
	}
}

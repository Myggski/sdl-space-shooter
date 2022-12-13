#pragma once

#include <bitset>

#include "component.h"

struct SDL_Texture;

namespace ecs
{
	namespace components
	{
		enum class rendering_layers : size_t
		{
			foreground,
			player,
			laser,
			enemy,
			background,
		};

		struct texture : component<texture, component_types::texture>
		{
		public:
			texture();
			texture(SDL_Texture* image, float width, float height, std::bitset<5> rendering_layer = static_cast<size_t>(rendering_layers::background));

			SDL_Texture* image;
			float width;
			float height;
			std::bitset<5> rendering_layer;
			size_t z_index;
		};
	}
}

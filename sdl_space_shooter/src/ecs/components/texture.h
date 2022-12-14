#pragma once

#include <bitset>

#include "component.h"

struct SDL_Texture;

namespace ecs
{
	namespace components
	{
		struct texture : component<texture, component_types::texture>
		{
		public:
			texture();
			texture(SDL_Texture* image, float width, float height);

		public:
			SDL_Texture* image;

			float width;
			float height;;
		};
	}
}

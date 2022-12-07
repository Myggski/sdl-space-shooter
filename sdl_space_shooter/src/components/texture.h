#pragma once

#include "components.h"
#include "ecs/component.h"

struct SDL_Texture;

namespace components
{
	struct texture : ecs::component<texture, component_type::texture>
	{
	public:
		texture();
		texture(SDL_Texture* image, float width, float height);

		SDL_Texture* image;
		float width;
		float height;
	};
}
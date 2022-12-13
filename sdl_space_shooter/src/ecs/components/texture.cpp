#include "pch.h"

#include "texture.h"

namespace ecs
{
	namespace components
	{
		texture::texture() : image(nullptr), width(0), height(0), rendering_layer(static_cast<size_t>(rendering_layers::background)), z_index(0) { }
		texture::texture(SDL_Texture* image, float width, float height, std::bitset<5> rendering_layer) : image(image), width(width), height(height), rendering_layer(rendering_layer),
			z_index(0)
		{
		}
	}
}

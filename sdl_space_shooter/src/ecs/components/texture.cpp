#include "pch.h"

#include "texture.h"

namespace ecs
{
	namespace components
	{
		texture::texture() : image(nullptr), width(0), height(0), z_index(0) { }
		texture::texture(SDL_Texture* image, float width, float height) : image(image), width(width), height(height),
			z_index(0)
		{
		}
	}
}

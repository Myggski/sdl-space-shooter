#include "pch.h"

#include "texture.h"

namespace components
{
	texture::texture() : image(nullptr), width(0), height(0) { }
	texture::texture(SDL_Texture* image, float width, float height) : image(image), width(width), height(height) {}
}

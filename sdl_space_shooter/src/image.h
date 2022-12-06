#pragma once

#include <SDL_video.h>
#include <SDL.h>


namespace image
{
	SDL_Texture* get_image(const char* const file_path, SDL_Renderer* renderer);
	void draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* image_rect);
}
#pragma once

#include <map>
#include <SDL_video.h>
#include <SDL.h>

namespace application
{
	class texture_manager
	{
	public:
		~texture_manager();
		SDL_Texture* get_image(const char* const file_path, SDL_Renderer* renderer);
		static void draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* image_rect);
	private:
		std::map<const char*, SDL_Texture*> textures;
	};
	
}

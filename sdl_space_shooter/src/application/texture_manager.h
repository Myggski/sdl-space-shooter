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
		void init(SDL_Renderer* renderer);
		SDL_Texture* get_image(std::string file_path);
		static void draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* image_rect, double angle = 0);
	private:
		SDL_Renderer* renderer;
		std::unordered_map<std::string, SDL_Texture*> textures;
	};
	
}

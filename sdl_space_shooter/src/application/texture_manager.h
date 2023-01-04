#pragma once

#include <map>
#include <SDL_video.h>
#include <SDL.h>

namespace application
{
	struct pixel_data {
	public:
		inline void clear() const
		{
			SDL_FreeSurface(surface);
		}

	public:
		SDL_Surface* surface;
		Uint32* pixels;
		SDL_PixelFormat* format;
		int pitch;
		int width;
		int height;
	};

	class texture_manager
	{
	public:
		~texture_manager();

		void init(SDL_Renderer* renderer);
		SDL_Texture* get_image(const std::string& file_path);

		static pixel_data get_pixels(const std::string& file_path);
		static void draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect& image_rect, double angle = 0);

	private:
		SDL_Renderer* renderer;
		std::unordered_map<std::string, SDL_Texture*> textures;
	};
}

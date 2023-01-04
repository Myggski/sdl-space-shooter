#include "pch.h"

#include "texture_manager.h"
#include <SDL.h>

namespace application
{
    texture_manager::~texture_manager()
    {
        while(!textures.empty())
        {
	        const auto texture = textures.begin();
            SDL_DestroyTexture(texture->second);
            textures.erase(texture->first);
        }
    }

    void texture_manager::init(SDL_Renderer* renderer)
    {
        this->renderer = renderer;
    }

	SDL_Texture* texture_manager::get_image(const std::string& file_path)
    {
        if (textures.contains(file_path)) { return textures.at(file_path); }

        SDL_Surface* surface = IMG_Load(file_path.c_str());
        assert(surface);
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        assert(texture);

        SDL_FreeSurface(surface);
        textures[file_path] = texture;

        return texture;
    }

	void texture_manager::draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect& image_rect, double angle)
    {
	    const SDL_FPoint center { image_rect.w / 2.f, image_rect.h / 2.f };
        SDL_RenderCopyExF(renderer, texture, nullptr, &image_rect, angle, &center, SDL_FLIP_NONE);
    }

    pixel_data texture_manager::get_pixels(const std::string& file_path)
	{
        SDL_Surface* surface = IMG_Load(file_path.c_str());
        assert(surface);

        const bool locked_surface = SDL_LockSurface(surface) >= 0;
        assert(locked_surface);

        Uint32* pixels = static_cast<Uint32*>(surface->pixels);
        const pixel_data pixel_data{ surface, pixels, surface->format, surface->pitch, surface->w, surface->h };

        SDL_UnlockSurface(surface);

        return pixel_data;
    }
}

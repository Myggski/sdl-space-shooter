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

	SDL_Texture* texture_manager::get_image(const char* const file_path, SDL_Renderer* renderer)
    {
        if (textures.contains(file_path)) { return textures.at(file_path); }

        SDL_Surface* surface = IMG_Load(file_path);
        assert(surface);
        
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        assert(texture);

        SDL_FreeSurface(surface);
        textures[file_path] = texture;

        return texture;
    }

    void texture_manager::draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* image_rect)
    {
        SDL_RenderCopyF(renderer, texture, nullptr, image_rect);
    }
}

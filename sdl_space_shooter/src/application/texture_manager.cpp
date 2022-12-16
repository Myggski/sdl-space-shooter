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

	SDL_Texture* texture_manager::get_image(std::string file_path)
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
}

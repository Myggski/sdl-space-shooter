#include "pch.h"

SDL_Texture* get_image(const char* const file_path, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(file_path);
    SDL_Texture* texture = nullptr;

    if (surface == nullptr)
    {
        printf("Failed to create surface, path: %s, SDL_Error: %s\n", file_path, SDL_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", file_path, SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(surface);
    }

    return texture;
}
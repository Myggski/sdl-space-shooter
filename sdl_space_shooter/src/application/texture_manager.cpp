#include "pch.h"

#include "texture_manager.h"
#include <SDL.h>

namespace application
{

#include "math.h"

    // Function to get the pixel value at the given coordinates on a surface
    Uint32 getPixel(SDL_Surface* surface, int x, int y)
    {
        // Get a pointer to the surface's pixel data
        Uint32* pixels = (Uint32*)surface->pixels;

        // Get the pixel at the given coordinates
        return pixels[y * surface->w + x];
    }

    // Function to set the pixel value at the given coordinates on a surface
    void putPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
    {
        // Get a pointer to the surface's pixel data
        Uint32* pixels = (Uint32*)surface->pixels;

        // Set the pixel at the given coordinates
        pixels[y * surface->w + x] = pixel;
    }

    // Function to rotate an SDL_Texture by a given angle
    SDL_Texture* rotateTexture(SDL_Renderer* renderer, SDL_Texture* texture, double angle)
    {
        // Get the width and height of the texture
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);

        // Calculate the sin and cos of the angle
        double s = sin(angle * M_PI / 180.0);
        double c = cos(angle * M_PI / 180.0);

        // Create a new temporary SDL_Surface to store the rotated version of the texture
        SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

        // Lock the surface
        SDL_LockSurface(surface);

        // Rotate the texture by the given angle
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                // Calculate the new coordinates for the current pixel
                int newX = x * c - y * s;
                int newY = x * s + y * c;

                // Make sure the new coordinates are within the bounds of the surface
                newX = std::min(std::max(newX, 0), w - 1);
                newY = std::min(std::max(newY, 0), h - 1);

                // Get the current pixel from the texture
                Uint32 pixel = getPixel(surface, x, y);

                // Set the pixel at the new coordinates on the surface
                putPixel(surface, newX, newY, pixel);
            }
        }

        // Unlock the surface
        SDL_UnlockSurface(surface);

        // Create a new texture from the rotated surface
        SDL_Texture* rotatedTexture = SDL_CreateTextureFromSurface(renderer, surface);

        // Free the rotated surface, since we no longer need it
        SDL_FreeSurface(surface);

        // Return the rotated texture
        return rotatedTexture;
    }


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

    void texture_manager::draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect* image_rect, double angle)
    {
	    const SDL_FPoint center { image_rect->w * 0.5f, image_rect->h * 0.5f };
        SDL_RenderCopyExF(renderer, texture, nullptr, image_rect, angle, &center, SDL_FLIP_NONE);
    }
}

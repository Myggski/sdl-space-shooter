#include "pch.h"

#include "font_manager.h"

namespace application
{
    font_manager& font_manager::get_instance()
    {
        static font_manager instance;
        return instance;
    }

    bool font_manager::init()
    {
        return TTF_Init() >= 0;
    }

    bool font_manager::load(const std::string& name, const std::string& file, int size)
    {
        TTF_Font* font = TTF_OpenFont(file.c_str(), size);
        
        if (!font)
        {
            return false;
        }
        
        fonts[name] = font;

        return true;
    }

    void font_manager::draw(SDL_Renderer* renderer, const std::string& font_name, const std::string& text, float x, float y) const
    {
        TTF_Font* font = fonts.at(font_name);
        if (!font) return;  // Error

        const SDL_Color color = { 255, 255, 255, 255 };  // White color
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int w, h;
        TTF_SizeText(font, text.c_str(), &w, &h);

        const SDL_FRect rect{ x, y, static_cast<float>(w), static_cast<float>(h) };

        SDL_RenderCopyF(renderer, texture, nullptr, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    TTF_Font* font_manager::get(const std::string& name) const
    {
        return fonts.at(name);
    }

    void font_manager::clear()
    {
        for (const auto& font : fonts) {
            TTF_CloseFont(font.second);
        }

        fonts.clear();
        TTF_Quit();
    }
}
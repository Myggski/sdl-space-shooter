#pragma once

#include <map>
#include <set>

namespace application
{
    struct keyboard_key_data
    {
    public:
	    bool is_held() const { return number_of_repeats > 0; }
        keyboard_key_data() = default;
	    keyboard_key_data(SDL_Scancode scancode);
        size_t number_of_repeats{ 0 };
        SDL_Scancode scancode{ SDL_SCANCODE_UNKNOWN };
    };

    class keyboard
    {
    private:
        std::map<SDL_Scancode, keyboard_key_data> keys;
    public:
        void on_key_pressed(SDL_Event event);
        void on_key_released(SDL_Event event);
        bool is_key_pressed(const SDL_Scancode scancode) const;
        bool is_key_released(const SDL_Scancode scancode) const;
        bool is_key_held(const SDL_Scancode scancode) const;
    };
}

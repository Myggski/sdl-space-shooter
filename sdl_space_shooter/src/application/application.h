#pragma once

#include "application_props.h"
#include "window_data.h"
#include "events.h"
#include "texture_manager.h"
#include "timer.h"
#include "keyboard.h"
#include "ecs/level.h"

namespace application
{
    class application
    {
    public:
        explicit application(const application_props& props);

        void init();
        void run();

        inline int get_width() const { return window_data.window_width; }
        inline int get_height() const { return window_data.window_height; }

    private:
        void quit(SDL_Renderer* renderer);

    private:
        bool is_running;

        window_data window_data;
        events events;
        keyboard keyboard;
        texture_manager texture_manager;
        timer time;
        game::level<ecs::MAX_COMPONENTS, ecs::MAX_SYSTEMS> level;
        SDL_Window* window;
        
        event_callback exit_application;
        event_callback on_key_down;
        event_callback on_key_released;
    };
}

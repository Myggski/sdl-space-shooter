#pragma once

#include "application_props.h"
#include "window_data.h"
#include "events.h"
#include "texture_manager.h"
#include "timer.h"
#include "keyboard.h"

namespace application
{
	class application {
	public:
		explicit application(const application_props& props);
		void init();
		void run_game();
		inline int get_width() const { return window_data.window_width; }
		inline int get_height() const { return window_data.window_height; }
	private:
		window_data window_data;
		events events;
		keyboard keyboard_input;
		texture_manager texture_manager;
		timer time;
		SDL_Window* window;
		bool is_running;
		void quit();
		event_callback exit_application = [this](SDL_Event)
		{
			is_running = false;
		};
		event_callback on_key_down = [&](SDL_Event event)
		{
			keyboard_input.on_key_pressed(event);
		};
		event_callback on_key_released = [&](SDL_Event event)
		{
			keyboard_input.on_key_released(event);
		};
	};
}

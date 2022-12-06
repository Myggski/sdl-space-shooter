#pragma once

#include "application_props.h"
#include "window_data.h"
#include "events.h"
#include "timer.h"
#include "input.h"

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
		timer time;
		input key_input;
		SDL_Window* window;
		bool is_running;
		void quit();
		event_callback exit_application = [this](SDL_Event)
		{
			is_running = false;
		};
		input_callback key_pressed_quit = [this](SDL_EventType event_type)
		{
			if (event_type == SDL_KEYUP)
			{
				is_running = false;
			}
		};
	};
}
#include "pch.h"

#include "application.h"
#include "font_manager.h"
#include "texture_manager.h"
#include "utils/interval.h"
#include "ecs/level.h"

namespace application
{
	application::application(const application_props& props)
		: window_data(props.name, props.window_width, props.window_height),
		events({}),
		keyboard({}),
		texture_manager({}),
		time({}),
		window(nullptr),
		is_running(true),
		level({ props.window_width, props.window_height, texture_manager }),
		exit_application([&](SDL_Event)
		{
			is_running = false;
		}),
		on_key_down([&](SDL_Event event)
		{
			keyboard.on_key_pressed(event);
		}),
		on_key_released([&](SDL_Event event)
		{
			keyboard.on_key_released(event);
		}) { }

	void application::init()
	{
		assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

		window = SDL_CreateWindow(
			window_data.name,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			window_data.window_width,
			window_data.window_height,
			0
		);

		SDL_RaiseWindow(window);

		events.add_listener(SDL_QUIT, &exit_application);
		events.add_listener(SDL_KEYDOWN, &on_key_down);
		events.add_listener(SDL_KEYUP, &on_key_released);

		font_manager::get_instance().init();
		font_manager::get_instance().load("default", "resources/fonts/Silver.ttf", 64);
	}

	float total_time = 0;

	void application::run()
	{
		is_running = true;

		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(renderer, 46, 34, 47, 255);
		texture_manager.init(renderer);

		time.init();
		level.init(renderer, keyboard);

		while (is_running) {
			SDL_RenderClear(renderer);

			time.refresh_dt();
			events.pull();
			utils::interval::get_instance().update();

			level.update(time.delta_time);

			// Quick-fix restart
			if (keyboard.is_key_pressed(SDL_SCANCODE_R))
			{
				level.restart(keyboard);
			}

			SDL_RenderPresent(renderer);
		}

		quit(renderer);
	}

	void application::quit(SDL_Renderer* renderer)
	{
		SDL_DestroyRenderer(renderer);

		events.clear();

		SDL_DestroyWindow(window);
		window = nullptr;

		IMG_Quit();
		SDL_Quit();
	}
}

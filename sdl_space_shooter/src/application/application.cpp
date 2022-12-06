#include "pch.h"
#include "application.h"
#include "input.h"
#include "image.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "components/position.h"
#include "components/velocity.h"
#include "systems/physics_system.h"

namespace application
{
	application::application(const application_props& props)
		: window_data(props.name, props.window_width, props.window_height), events({}), time({}), key_input({}),
		  window(nullptr), is_running(true) { }


	void application::init()
	{
		const bool initialization = SDL_Init(SDL_INIT_EVERYTHING);
		const bool initialized = initialization == 0;

		assert(initialized, "SDL was not able to initialize correctly");

		window = SDL_CreateWindow(
			window_data.name,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			window_data.window_width,
			window_data.window_height,
			0
		);

		SDL_RaiseWindow(window);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

		events.add_listener(SDL_QUIT, &exit_application);
		events.setup_input_events(&(key_input.invoke_key_press));

		key_input.add_listener(SDL_SCANCODE_Q, &key_pressed_quit);
	}

	void application::run_game()
	{
		is_running = true;

		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);
		SDL_Texture* bomb = image::get_image("resources/icon.png", renderer);
		const auto bomb_rect = new SDL_Rect{ 32, 32, 64, 64 };

		auto world = ecs::world<ecs::ComponentCount, ecs::SystemCount>();
		world.register_component<components::position>();
		world.register_component<components::velocity>();

		auto system = world.create_system<ecs::physics_system>(world);

		const auto entity = world.create_entity();
		world.add_component<components::position>(entity, components::position(0, 0));
		world.add_component<components::velocity>(entity, components::velocity(1, 0));

		time.init();

		while (is_running) {
			time.refresh_dt();
			events.pull();

			
			// update
			system->update(time.delta_time);

			SDL_RenderClear(renderer);


			// draw
			image::draw_texture(renderer, bomb, bomb_rect);
			
			SDL_RenderPresent(renderer);
		}

		SDL_DestroyRenderer(renderer);
		quit();
	}

	void application::quit()
	{
		key_input.clear();
		events.clear();

		SDL_DestroyWindow(window);
		window = nullptr;

		IMG_Quit();
		SDL_Quit();
	}

}
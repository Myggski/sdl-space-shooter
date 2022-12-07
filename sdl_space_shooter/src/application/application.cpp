#include "pch.h"
#include "application.h"
//#include "input.h"
#include "texture_manager.h"
#include "components/input.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "components/position.h"
#include "components/texture.h"
#include "components/velocity.h"
#include "systems/draw_system.h"
#include "systems/input_system.h"
#include "systems/physics_system.h"

namespace application
{
	application::application(const application_props& props)
		: window_data(props.name, props.window_width, props.window_height), events({}), keyboard_input({}), time({}), window(nullptr), is_running(true), texture_manager({}) { }


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
		events.add_listener(SDL_KEYDOWN, &on_key_down);
		events.add_listener(SDL_KEYUP, &on_key_released);
	}

	void application::run_game()
	{
		is_running = true;

		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);

		auto world = ecs::world<ecs::ComponentCount, ecs::SystemCount>();
		world.register_component<components::input>();
		world.register_component<components::position>();
		world.register_component<components::velocity>();
		world.register_component<components::texture>();

		const auto input_system = world.create_system<ecs::input_system>(world, keyboard_input);
		const auto physics_system = world.create_system<ecs::physics_system>(world);
		const auto draw_system = world.create_system<ecs::draw_system>(world, renderer);

		const auto entity = world.create_entity();
		world.add_component<components::input>(entity, components::input(
			SDL_SCANCODE_W, 
			SDL_SCANCODE_S, 
			SDL_SCANCODE_A, 
			SDL_SCANCODE_D
		));
		world.add_component<components::position>(entity, components::position(0, 0)); // start position
		world.add_component<components::velocity>(entity, components::velocity(0, 0)); // start velocity
		world.add_component<components::texture>(entity, components::texture(texture_manager.get_image("resources/icon.png", renderer), 64.f, 64.f)); // start velocity

		time.init();

		while (is_running) {
			time.refresh_dt();
			events.pull();

			
			// update
			input_system->update(time.delta_time);
			physics_system->update(time.delta_time);
			

			SDL_RenderClear(renderer);

			draw_system->update(time.delta_time);

			SDL_RenderPresent(renderer);
		}

		SDL_DestroyRenderer(renderer);
		quit();
	}

	void application::quit()
	{
		events.clear();

		SDL_DestroyWindow(window);
		window = nullptr;

		IMG_Quit();
		SDL_Quit();
	}

}

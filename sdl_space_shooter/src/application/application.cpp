#include "pch.h"

#include "application.h"
#include "texture_manager.h"
#include "ecs/components/input.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "ecs/systems/draw_system.h"
#include "ecs/systems/input_system.h"
#include "ecs/systems/physics_system.h"

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

	float randd() {
		return static_cast<float>(rand()) / (RAND_MAX + 1.0);
	}

	void application::run_game()
	{
		is_running = true;

		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);

		auto world = ecs::world<ecs::MAX_COMPONENTS, ecs::MAX_SYSTEMS>();
		world.reserve(1024 * 10);
		world.register_component<ecs::components::input>();
		world.register_component<ecs::components::position>();
		world.register_component<ecs::components::velocity>();
		world.register_component<ecs::components::texture>();

		world.create_system<ecs::systems::input_system>(world, keyboard_input);
		world.create_system<ecs::systems::physics_system>(world);
		world.create_system<ecs::systems::draw_system>(world, renderer);

		const auto entity = world.create_entity();
		world.add_component<ecs::components::position>(entity, ecs::components::position(0, 0)); // start position
		world.add_component<ecs::components::velocity>(entity, ecs::components::velocity(0, 0)); // start velocity
		world.add_component<ecs::components::texture>(entity, ecs::components::texture(texture_manager.get_image("resources/player.png", renderer), 64.f, 64.f)); // start velocity
		world.add_component<ecs::components::input>(entity, ecs::components::input(
			SDL_SCANCODE_W,
			SDL_SCANCODE_S,
			SDL_SCANCODE_A,
			SDL_SCANCODE_D
		));

		time.init();
		while (is_running) {
			time.refresh_dt();
			events.pull();

			SDL_RenderClear(renderer);

			world.update(time.delta_time);

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

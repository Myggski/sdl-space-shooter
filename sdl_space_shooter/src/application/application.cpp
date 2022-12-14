#include "pch.h"

#include "application.h"
#include "texture_manager.h"
#include "ecs/components/input.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/damage.h"
#include "ecs/components/health.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "ecs/components/removal_timer.h"
#include "ecs/components/rotation.h"
#include "ecs/entities/enemy.h"
#include "ecs/entities/player.h"
#include "ecs/systems/collision.h"
#include "ecs/systems/damage_collision.h"
#include "ecs/systems/draw_system.h"
#include "ecs/systems/enemy_spawner.h"
#include "ecs/systems/player_input.h"
#include "ecs/systems/physics_system.h"
#include "ecs/systems/player_movement.h"
#include "ecs/systems/laser_spawner.h"
#include "ecs/systems/time_removal.h"

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
		time.init();
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(renderer, 23, 23, 23, 255);

		texture_manager.init(renderer);

		auto world = ecs::world<ecs::MAX_COMPONENTS, ecs::MAX_SYSTEMS>();
		world.reserve(ecs::MAX_ENTITIES);
		world.register_component<ecs::components::input>();
		world.register_component<ecs::components::position>();
		world.register_component<ecs::components::velocity>();
		world.register_component<ecs::components::texture>();
		world.register_component<ecs::components::box_collider>();
		world.register_component<ecs::components::damage>();
		world.register_component<ecs::components::health>();
		world.register_component<ecs::components::rotation>();
		world.register_component<ecs::components::layer>();
		world.register_component<ecs::components::removal_timer>();

		world.create_system<ecs::systems::player_input>(world, keyboard_input);
		world.create_system<ecs::systems::laser_spawner>(world, texture_manager);
		world.create_system<ecs::systems::player_movement>(world);
		world.create_system<ecs::systems::physics_system>(world);
		world.create_system<ecs::systems::collision>(world);
		world.create_system<ecs::systems::damage_collision>(world);
		world.create_system<ecs::systems::enemy_spawner>(world, texture_manager);
		world.create_system<ecs::systems::draw_system>(world, renderer);
		world.create_system<ecs::systems::time_removal>(world);

		// Background
		const auto background = world.create_entity();
		world.add_component<ecs::components::position>(background, ecs::components::position(0, 0));
		world.add_component<ecs::components::layer>(background, ecs::components::layer());
		world.add_component<ecs::components::texture>(background, ecs::components::texture(texture_manager.get_image("resources/background.png"), 1280.f, 720.f));

		// Player
		ecs::entities::create_player(world, texture_manager, ecs::components::position(64.f, 360.f));

		while (is_running) {
			time.refresh_dt();
			events.pull();
			interval::get_instance().update();

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

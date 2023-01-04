#pragma once

#include "ecs/components/input.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/damage.h"
#include "ecs/components/health.h"
#include "ecs/components/points.h"
#include "ecs/components/removal_timer.h"
#include "ecs/components/rotation.h"
#include "ecs/components/text.h"
#include "ecs/entities/enemy.h"
#include "ecs/entities/player.h"
#include "ecs/systems/collision.h"
#include "ecs/systems/damage_collision.h"
#include "ecs/systems/draw_system.h"
#include "ecs/systems/player_input.h"
#include "ecs/systems/physics_system.h"
#include "ecs/systems/player_velocity.h"
#include "ecs/systems/laser_spawner.h"
#include "ecs/systems/scoreboard.h"
#include "ecs/systems/enemy_spawner.h"
#include "ecs/systems/time_removal.h"
#include "world.h"
#include "wave_function_collapse/wave_function_collapse.h"

namespace game
{
	template<size_t component_count, size_t system_count>
	class level
	{
	public:
		level(int screen_width, int screen_height, application::texture_manager& texture_manager)
			: renderer(nullptr),
		      texture_manager(texture_manager),
			  wfc({screen_width, screen_height, 56, tile_types::cyber_circuit}),
			  world({screen_width, screen_height}) { }

		void init(SDL_Renderer* renderer, application::keyboard& keyboard)
		{
			this->renderer = renderer;
			this->world.reserve(ecs::MAX_ENTITIES);

			register_components();

			//create_systems(renderer, texture_manager, keyboard);

			//ecs::entities::create_player(world, texture_manager, ecs::components::position(128.f, 360.f));

			//setup_scoreboard();
		}

		void update(const float dt)
		{
			wfc.draw(renderer, texture_manager);

			if (wfc.run())
			{
				world.update(dt);
			}
		}

		void restart(application::keyboard& keyboard)
		{
			clear();
			init(renderer, keyboard);
		}

		void clear()
		{
			wfc.clear();
			world.clear();
		}

	private:
		void register_components()
		{
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
			world.register_component<ecs::components::text>();
			world.register_component<ecs::components::points>();
		}

		void create_systems(SDL_Renderer* renderer, application::texture_manager& texture_manager, application::keyboard& keyboard)
		{
			world.create_system<ecs::systems::player_input>(world, keyboard);
			world.create_system<ecs::systems::collision>(world);
			world.create_system<ecs::systems::laser_spawner>(world, texture_manager);
			world.create_system<ecs::systems::player_velocity>(world);
			world.create_system<ecs::systems::physics_system>(world);
			world.create_system<ecs::systems::draw_system>(world, renderer);
			world.create_system<ecs::systems::damage_collision>(world);
			world.create_system<ecs::systems::enemy_spawner>(world, texture_manager);
			world.create_system<ecs::systems::time_removal>(world);
		}

		void setup_scoreboard()
		{
			const auto score_text = world.create_entity();
			world.add_component<ecs::components::position>(score_text, ecs::components::position(16, 16));
			world.add_component<ecs::components::layer>(score_text, ecs::components::layer(ecs::layer_types::player));
			world.add_component<ecs::components::text>(score_text, ecs::components::text("Score: 0"));

			world.create_system<ecs::systems::scoreboard>(world, score_text);
		}

	private:
		SDL_Renderer* renderer;
		application::texture_manager& texture_manager;
		wave_function_collapse wfc;
		ecs::world<component_count, system_count> world;
	};
}

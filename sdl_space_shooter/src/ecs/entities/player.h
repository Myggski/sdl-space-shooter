#pragma once

#include "application/texture_manager.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/damage.h"
#include "ecs/components/input.h"
#include "ecs/components/position.h"
#include "ecs/components/rotation.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"

namespace ecs
{
	namespace entities
	{
		constexpr float player_collision_percentage = 0.8888889f;
		constexpr float player_width = 90.f;
		constexpr float player_height = 160.f;
		constexpr float player_collision_width = player_width * player_collision_percentage;
		constexpr float player_collision_height = player_height * player_collision_percentage;
		constexpr float player_collision_offset_x = player_width * (1.f - player_collision_percentage) / 2.f;
		constexpr float player_collision_offset_y = player_height * (1.f - player_collision_percentage) / 2.f;

		template<std::size_t component_count, std::size_t system_count>
		inline entity create_player(world<component_count, system_count>& world, application::texture_manager& texture_manager, const components::position& position)
		{
			const auto entity = world.create_entity();
			world.add_component<components::position>(entity, position);
			world.add_component<components::velocity>(entity, components::velocity(0, 0));
			world.add_component<components::damage>(entity, components::damage(1));
			world.add_component<components::rotation>(entity, components::rotation(0));
			world.add_component<components::layer>(entity, components::layer(layer_types::player));
			world.add_component<components::box_collider>(entity, components::box_collider(
				player_collision_width,
				player_collision_height,
				0,
				0,
				static_cast<size_t>(layer_types::enemy)
			));
			world.add_component<components::texture>(entity, components::texture(
				texture_manager.get_image("resources/player.png"),
				player_width,
				player_height
			));
			world.add_component<components::input>(entity, components::input(
				SDL_SCANCODE_W,
				SDL_SCANCODE_S,
				SDL_SCANCODE_A,
				SDL_SCANCODE_D,
				SDL_SCANCODE_SPACE
			));

			return entity;
		}
	}
}

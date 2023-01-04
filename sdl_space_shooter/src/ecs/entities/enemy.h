#pragma once

#include "application/texture_manager.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/health.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"
#include "utils/random.h"
#include "ecs/components/layer.h"
#include "ecs/components/removal_timer.h"

namespace ecs
{
	namespace entities
	{
		constexpr float enemy_collision_percentage = 0.85f;
		constexpr float enemy_width = 70.f;
		constexpr float enemy_height = 75.f;
		constexpr float enemy_collision_width = enemy_width * enemy_collision_percentage;
		constexpr float enemy_collision_height = enemy_height * enemy_collision_percentage;
		constexpr float enemy_collision_offset_x = enemy_width * (1.f - enemy_collision_percentage) / 2.f;
		constexpr float enemy_collision_offset_y = enemy_height * (1.f - enemy_collision_percentage) / 2.f;

		static const std::array<std::string, 4> meteors{
			"resources/skull_0.png",
			"resources/skull_1.png",
			"resources/skull_2.png",
			"resources/skull_3.png"
		};

		static std::string get_random_enemy()
		{
			return meteors[utils::get_random(0, 3)];
		}

		template<std::size_t component_count, std::size_t system_count>
		inline entity create_enemy(world<component_count, system_count>& world, application::texture_manager& texture_manager, float y)
		{
			const auto entity = world.create_entity();
			world.add_component<components::position>(entity, components::position(1280 + enemy_width, y));
			world.add_component<components::box_collider>(entity, components::box_collider(
				enemy_collision_width,
				enemy_collision_height,
				enemy_collision_offset_x,
				enemy_collision_offset_y,
				static_cast<size_t>(layer_types::player)
			));
			world.add_component<components::health>(entity, components::health(1));
			world.add_component<components::removal_timer>(entity, components::removal_timer(8));
			world.add_component<components::layer>(entity, components::layer(layer_types::enemy));
			world.add_component<components::velocity>(entity, components::velocity(-2, 0));
			world.add_component<components::texture>(entity, components::texture(texture_manager.get_image(get_random_enemy()), enemy_width, enemy_height));

			return entity;
		}
	}
}

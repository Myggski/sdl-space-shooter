#pragma once

#include "application/texture_manager.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/health.h"
#include "ecs/components/position.h""
#include "ecs/components/points.h""
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"
#include "application/random.h"
#include "ecs/components/layer.h"
#include "ecs/components/removal_timer.h"

namespace ecs
{
	namespace entities
	{
		constexpr float enemy_collision_percentage = 0.85f;
		constexpr float enemy_width = 101.f;
		constexpr float enemy_height = 84.f;
		constexpr float enemy_collision_width = enemy_width * enemy_collision_percentage;
		constexpr float enemy_collision_height = enemy_height * enemy_collision_percentage;
		constexpr float enemy_collision_offset_x = enemy_width * (1.f - enemy_collision_percentage) / 2.f;
		constexpr float enemy_collision_offset_y = enemy_height * (1.f - enemy_collision_percentage) / 2.f;

		const std::array<std::string, 4> meteors{
			"resources/meteor0.png",
			"resources/meteor1.png",
			"resources/meteor2.png",
			"resources/meteor3.png"
		};

		inline std::string get_random_meteor()
		{
			return meteors[application::get_random(0, 3)];
		}

		template<std::size_t component_count, std::size_t system_count>
		inline entity create_enemy(world<component_count, system_count>& world, application::texture_manager& texture_manager, float y)
		{
			const auto entity = world.create_entity();
			world.add_component<ecs::components::position>(entity, ecs::components::position(1280 + 64, y));
			world.add_component<ecs::components::box_collider>(entity, ecs::components::box_collider(
				enemy_collision_width,
				enemy_collision_height,
				enemy_collision_offset_x,
				enemy_collision_offset_y,
				static_cast<size_t>(layers_types::player)
			));
			world.add_component<ecs::components::health>(entity, ecs::components::health(1));
			world.add_component<ecs::components::removal_timer>(entity, ecs::components::removal_timer(8));
			world.add_component<ecs::components::layer>(entity, ecs::components::layer(layers_types::enemy));
			world.add_component<ecs::components::velocity>(entity, ecs::components::velocity(-2, 0));
			world.add_component<ecs::components::texture>(entity, ecs::components::texture(texture_manager.get_image(get_random_meteor()), enemy_width, enemy_height));

			return entity;
		}
	}
}

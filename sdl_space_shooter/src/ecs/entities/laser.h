#pragma once

#include "application/texture_manager.h"
#include "ecs/entity.h"
#include "ecs/layer_types.h"
#include "ecs/world.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/damage.h"
#include "ecs/components/layer.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"
#include "ecs/components/rotation.h"
#include "ecs/components/removal_timer.h"

namespace ecs
{
	namespace entities
	{
		static const std::array<std::string, 2> lasers{
		"resources/laser0.png",
		"resources/laser1.png"
		};

		template<std::size_t component_count, std::size_t system_count>
		inline entity create_laser(world<component_count, system_count>& world, application::texture_manager& texture_manager, const ecs::components::position& position, bool zero = true)
		{
			constexpr float laser_collision_percentage = 1.f;
			constexpr float laser_width = 40.f;
			constexpr float laser_height = 50.f;
			constexpr float laser_collision_height = laser_height * laser_collision_percentage;
			constexpr float laser_collision_width = laser_width * laser_collision_percentage;
			constexpr float laser_collision_offset_x = laser_width * (1.f - laser_collision_percentage) / 2.f;
			constexpr float laser_collision_offset_y = laser_height * (1.f - laser_collision_percentage) / 2.f;

			const auto entity = world.create_entity();
			
			world.add_component<components::position>(entity, position);
			world.add_component<components::box_collider>(entity, components::box_collider(
				laser_collision_height,
				laser_collision_width, 
				laser_collision_offset_x,
				laser_collision_offset_y,
				static_cast<size_t>(layer_types::enemy)
			));
			world.add_component<components::damage>(entity, components::damage(1));
			world.add_component<components::removal_timer>(entity, components::removal_timer(1.5));
			world.add_component<components::velocity>(entity, components::velocity(8, 0));
			world.add_component<components::layer>(entity, components::layer(layer_types::player_laser));
			world.add_component<components::texture>(entity, components::texture(texture_manager.get_image(lasers[static_cast<int>(zero)]), laser_width, laser_height));
			

			return entity;
		}
	}
}

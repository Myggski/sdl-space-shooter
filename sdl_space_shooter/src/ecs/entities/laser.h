#pragma once

#include "application/texture_manager.h"
#include "ecs/entity.h"
#include "ecs/layers_types.h"
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

		template<std::size_t component_count, std::size_t system_count>
		inline entity create_laser(world<component_count, system_count>& world, application::texture_manager& texture_manager, const ecs::components::position& position)
		{
			constexpr float collision_percentage = 1.f;
			constexpr float width = 13.f;
			constexpr float height = 54.f;
			constexpr float collision_height = height * collision_percentage;
			constexpr float collision_width = width * collision_percentage;
			constexpr float collision_offset_x = -(height * 0.5f) + (width * 0.5f);
			constexpr float collision_offset_y = (height * 0.5f) - (width * 0.5f);

			const auto entity = world.create_entity();
			
			world.add_component<ecs::components::position>(entity, position);
			world.add_component<ecs::components::box_collider>(entity, ecs::components::box_collider(
				collision_height,
				collision_width, 
				collision_offset_x,
				collision_offset_y,
				static_cast<size_t>(layers_types::enemy)
			));
			world.add_component<ecs::components::damage>(entity, ecs::components::damage(1));
			world.add_component<ecs::components::removal_timer>(entity, ecs::components::removal_timer(4));
			world.add_component<ecs::components::rotation>(entity, ecs::components::rotation(-90));
			world.add_component<ecs::components::velocity>(entity, ecs::components::velocity(6, 0));
			world.add_component<ecs::components::layer>(entity, ecs::components::layer(layers_types::player_laser));
			world.add_component<ecs::components::texture>(entity, ecs::components::texture(texture_manager.get_image("resources/laser.png"), width, height));
			

			return entity;
		}
	}
}

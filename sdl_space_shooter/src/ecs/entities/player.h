#pragma once

#include "application/texture_manager.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/health.h"
#include "ecs/components/input.h"
#include "ecs/components/position.h"
#include "ecs/components/rotation.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"

namespace ecs
{
	namespace entities
	{
		inline entity create_player(world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::texture_manager& texture_manager, const components::position& position)
		{
			constexpr float width = 98.f;
			constexpr float height = 75.f;

			const auto entity = world.create_entity();
			world.add_component<ecs::components::position>(entity, position);
			world.add_component<ecs::components::velocity>(entity, ecs::components::velocity(0, 0));
			world.add_component<ecs::components::rotation>(entity, ecs::components::rotation(90));
			world.add_component<ecs::components::health>(entity, ecs::components::health(1));
			world.add_component<ecs::components::box_collider>(entity, ecs::components::box_collider(width, height)); 
			world.add_component<ecs::components::texture>(entity, ecs::components::texture(
				texture_manager.get_image("resources/player.png"),
				width,
				height
			));
			world.add_component<ecs::components::input>(entity, ecs::components::input(
				SDL_SCANCODE_W,
				SDL_SCANCODE_S,
				SDL_SCANCODE_A,
				SDL_SCANCODE_D
			));

			return entity;
		}
	}
}

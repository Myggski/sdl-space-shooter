#pragma once

#include "application/texture_manager.h"
#include "ecs/entity.h"
#include "ecs/world.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/damage.h"
#include "ecs/components/health.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/components/velocity.h"

namespace ecs
{
	namespace entities
	{
		inline entity create_enemy(world<MAX_COMPONENTS, MAX_SYSTEMS>& world, application::texture_manager& texture_manager, float y)
		{
			constexpr float width = 101.f;
			constexpr float height = 84.f;

			const auto entity = world.create_entity();
			world.add_component<ecs::components::position>(entity, ecs::components::position(1280 + 64, y));
			world.add_component<ecs::components::box_collider>(entity, ecs::components::box_collider(width, height));
			world.add_component<ecs::components::damage>(entity, ecs::components::damage(1));
			world.add_component<ecs::components::health>(entity, ecs::components::health(1));
			world.add_component<ecs::components::velocity>(entity, ecs::components::velocity(-1, 0));
			world.add_component<ecs::components::texture>(entity, ecs::components::texture(texture_manager.get_image("resources/meteor1.png"), width, height));

			return entity;
		}
	}
}

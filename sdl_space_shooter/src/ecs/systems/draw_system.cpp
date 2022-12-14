#include "pch.h"

#include "draw_system.h"
#include "application/texture_manager.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/world.h"
#include "ecs/components/layer.h"
#include "ecs/components/rotation.h"

namespace ecs
{
	namespace components
	{
		struct rotation;
	}
}

namespace ecs
{
    namespace systems
    {
        draw_system::draw_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, SDL_Renderer* renderer)
            : system(world), renderer(renderer), is_dirty(true)
        {
            set_all_requirements<components::position, components::texture, components::layer>();
            set_update([&](const float dt) { draw(dt); });
        }
        void draw_system::draw(const float dt)
        {
            if (is_dirty)
            {
                
                draw_entities = get_entities();
                std::unordered_map<ecs::entity, size_t> z_index_values;
                z_index_values.reserve(draw_entities.size());

                for (const auto& entity : draw_entities)
                {
                    z_index_values.emplace(entity, world.get_component<components::texture>(entity).z_index);
                }

                std::ranges::sort(draw_entities, [&](const entity& a, const entity& b) {
						auto d = z_index_values[a];
						auto e = z_index_values[b];
						return d < e;
                    });

                is_dirty = false;
            }

            for (const auto& entity : draw_entities)
            {
                auto [position, texture] = world.get_components<components::position, components::texture>(entity);
                const double angle = world.has_component<components::rotation>(entity)
                    ? world.get_component<components::rotation>(entity).angle
                    : 0.f;

                SDL_FRect texture_rect{ position.x, position.y, texture.width, texture.height };
                application::texture_manager::draw_texture(renderer, texture.image, &texture_rect, angle);
            }

            printf("FPS: %f - ENEMIES - %llu\n", 1.f / dt, draw_entities.size());
        }

        void draw_system::on_valid_entity_added(entity entity)
        {
            auto [texture, layer] = world.get_components<components::texture, components::layer>(entity);
            texture.z_index = static_cast<size_t>(entity) + (500000 * static_cast<size_t>(layer.layer_type));

            is_dirty = true;
        }

        void draw_system::on_valid_entity_removed(entity entity)
        {
            is_dirty = true;
        }
    }
}

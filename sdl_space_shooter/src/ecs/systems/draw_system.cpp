#include "pch.h"

#include "draw_system.h"
#include "application/texture_manager.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/world.h"
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
            set_all_requirements<components::position, components::texture>();
            set_update([&](const float dt) { draw(dt); });
        }
        void draw_system::draw(const float dt)
        {
            if (is_dirty)
            {
                draw_entities = get_entities();
                std::ranges::sort(draw_entities, [&](const entity& a, const entity& b) {
                    return a > b;
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


            printf("enemies: %d %f\n", draw_entities.size(), 1.f / dt);
        }

        void draw_system::on_valid_entity_added(entity entity)
        {
            auto texture = world.get_component<components::texture>(entity);
            texture.z_index = static_cast<unsigned long long>(entity) + (50000 * texture.rendering_layer.to_ullong());

            is_dirty = true;
        }

        void draw_system::on_valid_entity_removed(entity entity)
        {
            is_dirty = true;
        }
    }
}

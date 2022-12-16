#include "pch.h"

#include "draw_system.h"

#include "application/font_manager.h"
#include "application/texture_manager.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/world.h"
#include "ecs/components/box_collider.h"
#include "ecs/components/layer.h"
#include "ecs/components/rotation.h"
#include "ecs/components/text.h"
#include "application/math.h"
#include "collision/aabb.h"

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
            set_all_requirements<components::position, components::layer>();
            set_any_requirements<components::text, components::texture>();
            set_update([&](const float dt) { draw(dt); });
        }
        void draw_system::draw(const float dt)
        {
            try_sort_z_index();
            ;
            for (const auto& entity : draw_entities)
            {
                world.has_component<components::texture>(entity)
                    ? draw_texture(entity)
                    : draw_text(entity);
            }
        }

        void draw_system::draw_text(ecs::entity entity) const
        {
            auto [position, text] = world.get_components<components::position, components::text>(entity);
            font_manager::get_instance().draw(renderer, text.font_name, text.text_string, position.x, position.y);
        }

        void draw_system::draw_texture(ecs::entity entity) const
        {
            auto [position, texture, box_collider] = world.get_components<components::position, components::texture, components::box_collider>(entity);

            const double angle = world.has_component<components::rotation>(entity)
                ? world.get_component<components::rotation>(entity).angle
                : 0;

            SDL_FRect texture_rect{ position.x, position.y, texture.width, texture.height };
            application::texture_manager::draw_texture(renderer, texture.image, texture_rect, angle);
        }

        void draw_system::try_sort_z_index()
        {
            if (is_dirty)
            {
                draw_entities = get_entities();
                std::unordered_map<ecs::entity, size_t> z_index_values;
                z_index_values.reserve(draw_entities.size());

                for (const auto& entity : draw_entities)
                {
                    z_index_values.emplace(entity, world.get_component<components::layer>(entity).z_index);
                }

                std::ranges::sort(draw_entities, [&](const entity& a, const entity& b) {
                    auto d = z_index_values[a];
                    auto e = z_index_values[b];
                    return d < e;
                    });

                is_dirty = false;
            }
        }


        void draw_system::on_valid_entity_added(entity entity)
        {
            auto& layer = world.get_component<components::layer>(entity);
            layer.z_index = static_cast<size_t>(entity) + (500000 * static_cast<size_t>(layer.layer_type));

            is_dirty = true;
        }

        void draw_system::on_valid_entity_removed(entity entity)
        {
            is_dirty = true;
        }
    }
}

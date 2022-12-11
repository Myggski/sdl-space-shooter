#include "pch.h"

#include "draw_system.h"
#include "application/texture_manager.h"
#include "ecs/components/position.h"
#include "ecs/components/texture.h"
#include "ecs/world.h"

namespace ecs
{
    namespace systems
    {
        draw_system::draw_system(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world, SDL_Renderer* renderer)
            : system(world), renderer(renderer)
        {
            set_all_requirements<components::position, components::texture>();
            set_update([&](const float dt) { draw(dt); });
        }
        void draw_system::draw(const float dt) const
        {
            for (const auto& entity : get_entities())
            {
                auto [position, texture] = world.get_components<components::position, components::texture>(entity);

                SDL_FRect texture_rect{ position.x, position.y, texture.width, texture.height };
                application::texture_manager::draw_texture(renderer, texture.image, &texture_rect, 90.f);
            }
        }
    }
}

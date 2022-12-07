#include "pch.h"

#include "draw_system.h"

#include "application/texture_manager.h"
#include "components/position.h"
#include "components/texture.h"
#include "ecs/world.h"

namespace ecs
{
    extern SDL_FRect texture_rect{};
    draw_system::draw_system(ecs::world<ecs::ComponentCount, ecs::SystemCount>& world, SDL_Renderer* renderer) :
	    world(world), renderer(renderer)
    {
        set_requirements<components::position, components::texture>();
    }
    void draw_system::update(const float dt) const
    {
        for (const auto& entity : get_managed_entities())
        {
            auto [position, texture] = world.get_components<components::position, components::texture>(entity);

            texture_rect.x = position.x;
            texture_rect.y = position.y;
            texture_rect.w = texture.width;
            texture_rect.h = texture.height;

            application::texture_manager::draw_texture(renderer, texture.image, &texture_rect);

            printf("x: %f y: %f\n", position.x, position.y);
        }
    }
}

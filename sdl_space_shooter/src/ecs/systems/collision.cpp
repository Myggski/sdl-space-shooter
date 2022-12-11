#include "pch.h"

#include "collision.h"
#include "ecs/components/position.h"
#include "ecs/components/box_collider.h"
#include "ecs/world.h"
#include "collision/aabb.h"

namespace ecs
{
    namespace systems
    {
        const SDL_FRect collision::get_rect_data(entity entity) const
        {
            auto [position, box_collider] = world.get_components<components::position, components::box_collider>(entity);
            return ::collision::get_rect_data(position, box_collider);
        }

        collision::collision(ecs::world<MAX_COMPONENTS, MAX_SYSTEMS>& world)
            : system(world)
        {
            set_all_requirements<components::position, components::box_collider>();
        }

        void collision::on_valid_entity_added(entity entity)
        {
            world.insert_to_grid(entity, get_rect_data(entity));
        }

        void collision::on_valid_entity_removed(entity entity)
        {
            world.remove_from_grid(entity, get_rect_data(entity));
        }
    }
}

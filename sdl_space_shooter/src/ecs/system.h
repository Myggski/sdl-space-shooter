#pragma once

#include <bitset>
#include <vector>
#include "entity.h"

namespace ecs
{
    template<std::size_t ComponentCount, std::size_t SystemCount>
    class world;

    template<std::size_t ComponentCount, std::size_t SystemCount>
    class system
    {
    public:
        virtual ~system() = default;

    protected:
        template<typename ...Ts>
        void set_requirements()
        {
            (requirements.set(Ts::type), ...);
        }

        const std::vector<ecs::entity>& get_managed_entities() const
        {
            return managed_entities;
        }

        virtual void on_managed_entity_added([[maybe_unused]] ecs::entity entity)
        {

        }

        virtual void on_managed_entity_removed([[maybe_unused]] ecs::entity entity)
        {

        }

    private:
        friend ecs::world<ComponentCount, SystemCount>;

        std::bitset<ComponentCount> requirements;
        std::size_t type{};
        std::vector<ecs::entity> managed_entities;
        std::vector<ecs::entity_id>* entity_to_managed_entity = nullptr;

        void setup(std::size_t type, std::vector<ecs::entity_id>* entity_to_managed_entity)
        {
            this->type = type;
            this->entity_to_managed_entity = entity_to_managed_entity;
        }

        void on_entity_updated(ecs::entity entity, const std::bitset<ComponentCount>& components)
        {
            auto satisfied = (requirements & components) == requirements;
            auto managed = (*entity_to_managed_entity)[static_cast<entity_id>(entity)] != invalid_entity_id;
            if (satisfied && !managed)
                add_entity(entity);
            else if (!satisfied && managed)
                remove_entity(entity);
        }

        void on_entity_removed(ecs::entity entity)
        {
            if ((*entity_to_managed_entity)[entity] != invalid_entity_id)
                remove_entity(entity);
        }

        void add_entity(ecs::entity entity)
        {
            (*entity_to_managed_entity)[static_cast<entity_id>(entity)] = static_cast<ecs::entity_id>(managed_entities.size());
            managed_entities.emplace_back(entity);
            on_managed_entity_added(entity);
        }

        void remove_entity(ecs::entity entity)
        {
            on_managed_entity_removed(entity);
            const auto index = (*entity_to_managed_entity)[static_cast<entity_id>(entity)];
            (*entity_to_managed_entity)[static_cast<entity_id>(managed_entities.back())] = index;
            (*entity_to_managed_entity)[static_cast<entity_id>(entity)] = invalid_entity_id;
            managed_entities[index] = managed_entities.back();
            managed_entities.pop_back();
        }
    };

}
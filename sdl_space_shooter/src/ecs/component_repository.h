#pragma once

#include "entity.h"
#include <bitset>

namespace ecs
{
    template<typename T, size_t component_count, size_t system_count>
    class component_repository : public ecs::base_component_repository
    {
    public:
        component_repository(std::vector<entity_id>& entity_to_component,
            std::vector<std::bitset<component_count>>& entity_to_bitset) :
            entity_to_component(entity_to_component), entity_to_bitset(entity_to_bitset) {}

        virtual void reserve(std::size_t size) override
        {
            components.reserve(size);
            component_to_entity.reserve(size);
        }

        T& get(ecs::entity entity)
        {
            return components[entity_to_component[static_cast<entity_id>(entity)]];
        }
        const T& get(ecs::entity entity) const
        {
            return components[entity_to_component[entity]];
        }
        template<typename... Args>
        void add(ecs::entity entity, Args&&... args)
        {
            const auto index = static_cast<entity_id>(components.size());
            components.emplace_back(std::forward<Args>(args)...);
            component_to_entity.emplace_back(entity);
            entity_to_component[static_cast<entity_id>(entity)] = index;
            entity_to_bitset[static_cast<entity_id>(entity)][T::type] = true;
        }
        void remove(ecs::entity entity)
        {
            entity_to_bitset[static_cast<entity_id>(entity)][T::type] = false;
            const auto index = entity_to_component[static_cast<entity_id>(entity)];

            // Update components
            components[index] = std::move(components.back());
            components.pop_back();

            // Update entity_to_component
            entity_to_component[static_cast<entity_id>(component_to_entity.back())] = index;

            // Update component_to_entity
            component_to_entity[index] = component_to_entity.back();
            component_to_entity.pop_back();
        }
        virtual bool try_remove(ecs::entity entity) override
        {
            if (!entity_to_bitset[static_cast<entity_id>(entity)][T::type])
            {
                return false;
            }

            remove(entity);
            return true;
        }
        ecs::entity get_owner(const T& component) const
        {
            auto begin = components.data();
            auto index = static_cast<std::size_t>(&component - begin);
            return component_to_entity[index];
        }

    private:
        std::vector<T> components;
        std::vector<ecs::entity> component_to_entity;
        std::vector<entity_id>& entity_to_component;
        std::vector<std::bitset<component_count>>& entity_to_bitset;
    };
}


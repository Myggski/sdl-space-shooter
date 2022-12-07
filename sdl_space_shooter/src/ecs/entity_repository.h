#pragma once

#include "entity.h"
#include <bitset>
#include <numeric>
#include <vector>
#include <array>

namespace ecs
{
    template<size_t component_count, size_t system_count>
    class entity_repository
    {
    public:
        void reserve(std::size_t size)
        {
            free_entities.resize(size);
            std::fill(free_entities.begin(), free_entities.end(), static_cast<ecs::entity>(0));
            entity_to_bitset.resize(size);

            for (auto& entity_to_component : entity_to_components)
            {
                entity_to_component.resize(size);
            }

            for (auto& entity_to_managed_entity : entity_to_managed_entities)
            {
                entity_to_managed_entity.resize(size);
            }
        }
        inline std::vector<std::bitset<component_count>>& get_entity_to_bitset()
        {
            return entity_to_bitset;
        }
        const std::bitset<component_count>& get_bitset(ecs::entity entity) const
        {
            return entity_to_bitset[static_cast<entity_id>(entity)];
        }
        std::vector<entity_id>& get_entity_to_component(std::size_t type)
        {
            return entity_to_components[type];
        }
        std::vector<entity_id>& get_entity_to_managed_entity(std::size_t type)
        {
            return entity_to_managed_entities[type];
        }
        entity create()
        {
            if (free_entities.empty())
            {
	            const ecs::entity entity(static_cast<ecs::entity>(entity_to_bitset.size()));
                entity_to_bitset.emplace_back();

                for (auto& entity_to_component : entity_to_components)
                {
                    entity_to_component.emplace_back();
                }

                for (auto& entity_to_managed_entity : entity_to_managed_entities)
                {
                    entity_to_managed_entity.emplace_back(invalid_entity_id);
                }

                return entity;
            }

            const ecs::entity entity = free_entities.back();
            free_entities.pop_back();
            entity_to_bitset[static_cast<entity_id>(entity)].reset();

            for (auto& entity_to_managed_entity : entity_to_managed_entities)
            {
                entity_to_managed_entity[static_cast<entity_id>(entity)] = invalid_entity_id;
            }

            return entity;
        }
        void remove(ecs::entity entity)
        {
            free_entities.push_back(entity);
        }

    private:
        std::vector<std::bitset<component_count>> entity_to_bitset{};
        std::array<std::vector<entity_id>, component_count> entity_to_components;
        std::array<std::vector<entity_id>, system_count> entity_to_managed_entities;
        std::vector<ecs::entity> free_entities{};
    };

}

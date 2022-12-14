#pragma once

#include <bitset>
#include <numeric>
#include <vector>
#include <array>

namespace ecs
{
    namespace entities
    {
        template<size_t component_count>
        class entity_repository
        {
        public:
	        void reserve(size_t number_of_entities)
            {
                number_of_entities = number_of_entities;
                killed_entities.reserve(number_of_entities);
                std::iota(std::begin(killed_entities), std::end(killed_entities), 0);
                alive_entities.resize(number_of_entities);

                for (auto& entity_to_component : entity_to_component_index)
                {
                    entity_to_component.resize(number_of_entities);
                }
            }

            void clear()
	        {
                killed_entities.clear();
                alive_entities.clear();

                for (auto& entity_to_component : entity_to_component_index)
                {
                    entity_to_component.clear();
                }
	        }

            std::vector<std::bitset<component_count>>& get_entities()
            {
                return alive_entities;
            }

            const std::bitset<component_count>& get_entity(entity entity) const
            {
                return alive_entities[entity];
            }
            std::vector<index>& get_entity_to_component(size_t type)
            {
                return entity_to_component_index[type];
            }

            entity create()
            {
                auto entity = ecs::entity();

                if (killed_entities.empty())
                {
                    entity = static_cast<ecs::entity>(alive_entities.size());
                    alive_entities.emplace_back();

                    for (auto& entity_to_component : entity_to_component_index)
                    {
                        entity_to_component.emplace_back();
                    }

                    return entity;
                }
                
                entity = killed_entities.back();
                killed_entities.pop_back();
                alive_entities[entity].reset();

                return entity;
            }
            void remove(entity entity)
            {
                killed_entities.push_back(entity);
            }

            bool is_alive(entity entity) const
	        {
                return std::ranges::find(killed_entities, entity) == killed_entities.end();
	        }

        private:
            std::vector<std::bitset<component_count>> alive_entities{};
            std::array<std::vector<index>, component_count> entity_to_component_index;
            std::vector<entity> killed_entities{};
            size_t number_of_entities;
        };

    }
}

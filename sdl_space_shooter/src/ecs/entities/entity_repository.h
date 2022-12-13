#pragma once

#include <bitset>
#include <numeric>
#include <vector>
#include <array>
#include "ecs/world.h"

namespace ecs
{
    namespace entities
    {
        template<size_t component_count>
        class entity_repository
        {
        public:
	        /**
	         * \brief Reserves memory
	         * \param number_of_entities Number of entities to reserve
	         */
	        void reserve(size_t number_of_entities)
            {
                killed_entities.resize(number_of_entities);
                std::iota(std::begin(killed_entities), std::end(killed_entities), 0);
                alive_entities.resize(number_of_entities);

                for (auto& entity_to_component : entity_to_component_index)
                {
                    entity_to_component.resize(number_of_entities);
                }
            }

            /**
             * \brief Getter that return a list of entities
             * \return List of entities with bitset of components
             */
            inline std::vector<std::bitset<component_count>>& get_entities()
            {
                return alive_entities;
            }

            /**
             * \brief Getter that returns a specific entity
             * \param entity Entity is a bitset of components that defines the entity type
             * \return 
             */
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
                if (killed_entities.empty())
                {
                    const entity entity(alive_entities.size());
                    alive_entities.emplace_back();

                    for (auto& entity_to_component : entity_to_component_index)
                    {
                        entity_to_component.emplace_back();
                    }

                    return entity;
                }

                const entity entity = killed_entities.back();
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
        };

    }
}

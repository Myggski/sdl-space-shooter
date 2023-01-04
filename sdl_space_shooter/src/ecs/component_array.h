#pragma once

#include "ecs/entity.h"
#include <bitset>
#include "ecs/base_component_array.h"

namespace ecs
{
        template<typename T, size_t component_count>
        class component_array final : public base_component_array
        {
        public:
            component_array(std::vector<index>& entity_to_component_index,
                std::vector<std::bitset<component_count>>& entity_components) :
                entity_to_component_index(entity_to_component_index), entity_components(entity_components) {}

            void reserve(std::size_t number_of_entities) override
            {
                component_values.reserve(number_of_entities);
                component_index_to_entity.reserve(number_of_entities);
            }

            void clear() override
            {
                component_values.clear();
                component_index_to_entity.clear();
                entity_to_component_index.clear();
            }

            T& get(entity entity)
            {
                auto index = entity_to_component_index[entity];
                return component_values[index];
            }

            const T& get(entity entity) const
            {
                return component_values[entity_to_component_index[entity]];
            }

            template<typename... Args>
            void add(entity entity, Args&&... args)
            {
                const auto component_index = component_values.size();
                component_values.emplace_back(std::forward<Args>(args)...);
                component_index_to_entity.emplace_back(entity);
                entity_to_component_index[entity] = component_index;
                entity_components[entity][T::type] = true;
            }
            void remove(entity entity)
            {
                entity_components[entity][T::type] = false;
                const auto component_index = entity_to_component_index[entity];

                // Update component_values
                component_values[component_index] = std::move(component_values.back());
                component_values.pop_back();

                // Update entity_to_component_index
                entity_to_component_index[component_index_to_entity.back()] = component_index;

                // Update component_index_to_entity
                component_index_to_entity[component_index] = component_index_to_entity.back();
                component_index_to_entity.pop_back();
            }
            bool try_remove(entity entity) override
            {
                const bool has_component = entity_components[entity][T::type];

                if (has_component)
                {
                    remove(entity);
                }

                return has_component;
            }

        private:
            std::vector<T> component_values;
            std::vector<entity> component_index_to_entity;
            std::vector<index>& entity_to_component_index;
            std::vector<std::bitset<component_count>>& entity_components;
        };
}

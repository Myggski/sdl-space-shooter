#pragma once

#include <bitset>
#include <vector>
#include "ecs/entity.h"

namespace ecs
{
    using update_func = std::function<void(const float dt)>;

    template<std::size_t component_count, std::size_t system_count>
    class world;
    
    template<std::size_t component_count, std::size_t system_count>
    class system
    {
    public:
        system(world<component_count, system_count>& world) : world(world) {}
        virtual ~system() = default;

    protected:
        ecs::world<component_count, system_count>& world;

        void set_update(update_func update_func)
        {
            update = update_func;
        }

        /**
         * \brief Adds the component types as requirements for the system
         * \tparam Ts Is a list of components
         */
        template<typename ...Ts>
        void set_all_requirements()
        {
            (all_requirements.set(Ts::type), ...);
        }

        template<typename ...Ts>
        void set_any_requirements()
        {
            (any_requirements.set(Ts::type), ...);
        }

        /**
         * \brief Returns valid entities
         * \return Valid entities that is meeting the component requirements for the system
         */
        const std::vector<entity>& get_entities() const
        {
            return valid_entities;
        }

        virtual void on_valid_entity_added([[maybe_unused]] entity entity) { }
        virtual void on_valid_entity_removed([[maybe_unused]] entity entity) { }
    private:
        friend ecs::world<component_count, system_count>;

        update_func update;
        std::bitset<component_count> all_requirements;
        std::bitset<component_count> any_requirements;
        std::size_t system_id{};
        std::vector<entity> valid_entities;
        std::unordered_map<entity, index> entity_to_valid_entity;

        /**
         * \brief Sets the id of the system
         * \param system_id is the index of the system
         */
        void setup(std::size_t system_id)
        {
            this->system_id = system_id;
        }

        /**
         * \brief Adds or remove the entity from the system when the entity has changed its signature
         * \param entity The entity that has changed
         * \param components The components for the entity
         */
        void on_entity_updated(entity entity, const std::bitset<component_count>& components)
        {
            bool satisfied = true;
            const bool managed = entity_to_valid_entity.contains(entity);

            if (any_requirements.any())
            {
                satisfied = (any_requirements & components) != 0;
            }

            if (all_requirements.any() && satisfied)
            {
	            satisfied = (all_requirements & components) == all_requirements;
            }
            

            if (satisfied && !managed)
            {
                add_entity(entity);
            }
            else if (!satisfied && managed)
            {
                remove_entity(entity);
            }
        }

        /**
         * \brief Removes the entity from the system
         * \param entity The entity that has been removed
         */
        void on_entity_removed(entity entity)
        {
            remove_entity(entity);
        }

        /**
         * \brief Adds the entity to the system
         * \param entity The entity that is being added
         */
        void add_entity(entity entity)
        {
            entity_to_valid_entity[entity] = valid_entities.size();
            valid_entities.emplace_back(entity);
            on_valid_entity_added(entity);
        }

        /**
         * \brief Removes the entity if it's in the system
         * \param entity The entity that is being removed
         */
        void remove_entity(entity entity)
        {
            if (entity_to_valid_entity.contains(entity))
            {
                on_valid_entity_removed(entity);

                valid_entities[entity_to_valid_entity.at(entity)] = valid_entities.back();
                entity_to_valid_entity.erase(entity);
                valid_entities.pop_back();
            }
        }
    };
}
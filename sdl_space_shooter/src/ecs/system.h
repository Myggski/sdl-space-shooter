#pragma once

#include <bitset>
#include <vector>
#include "ecs/entity.h"

namespace ecs
{
    template<std::size_t component_count, std::size_t system_count>
    class world;

    template<std::size_t component_count, std::size_t system_count>
    class system
    {
    public:
        system(world<component_count, system_count>& world) : world(world) {}
        virtual ~system() = default;
        virtual void update(float dt) const = 0;

    protected:
        ecs::world<component_count, system_count>& world;
        /**
         * \brief Adds the component types as requirements for the system
         * \tparam Ts Is a list of components
         */
        template<typename ...Ts>
        void set_requirements()
        {
            (requirements.set(Ts::type), ...);
        }

        /**
         * \brief Returns valid entities
         * \return Valid entities that is meeting the component requirements for the system
         */
        const std::vector<entity>& get_entities() const
        {
            return valid_entities;
        }

        virtual void on_entity_added([[maybe_unused]] entity entity) { }
        virtual void on_valid_entity_removed([[maybe_unused]] entity entity) { }
    private:
        friend ecs::world<component_count, system_count>;

        std::bitset<component_count> requirements;
        std::size_t system_id{};
        std::vector<entity> valid_entities;

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
            auto satisfied = (requirements & components) == requirements;
            const auto found_entity = std::ranges::find(valid_entities, entity);
            auto managed = found_entity != valid_entities.end() && *found_entity != invalid_entity_id;
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
            valid_entities.emplace_back(entity);
            on_entity_added(entity);
        }

        /**
         * \brief Removes the entity if it's in the system
         * \param entity The entity that is being removed
         */
        void remove_entity(entity entity)
        {
            const auto found_entity = std::ranges::find(valid_entities, entity);

            if (found_entity != valid_entities.end())
            {
                on_valid_entity_removed(entity);

                valid_entities[found_entity - valid_entities.begin()] = valid_entities.back();
                valid_entities.pop_back();
            }
        }
    };
}
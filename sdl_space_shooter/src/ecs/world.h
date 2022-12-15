#pragma once

#include "collision/spatial_grid.h"
#include "ecs/base_component_array.h"
#include "ecs/component_array.h"
#include "ecs/entities/entity_repository.h"
#include "ecs/system.h"
#include "ecs/setup_data.h"

namespace ecs
{
    template<std::size_t component_count, std::size_t system_count>
    class world
    {
    public:
        world(int width, int height) : width(width), height(height), spatial_grid(MAX_ENTITIES, width, height) {}
	    /**
	     * \brief Creates a component array of the specific type of component
	     * \tparam T is of class component
	     */
	    template<typename T>
        void register_component()
        {
            check_component_type<T>();
            components[T::type] = std::make_unique<component_array<T, component_count>>(
                entity_repository.get_entity_to_component(T::type), entity_repository.get_entities());
        }

	    /**
	     * \brief Adding system to list of systems
	     * \tparam T System type
	     * \return The system that's being added to the world
	     */
	    template<typename T, typename ...Args>
        T* create_system(Args&& ...args)
        {
            // uses std::forward to avoid reference collapsing
            // it will either copy or move the args depending on reference type
            auto& system = systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
            system->setup(systems.size(), MAX_ENTITIES);
            
            if (system->update)
            {
                systems_updates.emplace_back(system->update);
            }

            return static_cast<T*>(system.get());
        }

	    /**
	     * \brief Reserves size for all the components and entities
	     * \param number_of_entities How many entities that the world should contain
	     */
	    void reserve(std::size_t number_of_entities)
        {
            for (size_t i = 0; i < component_count; ++i)
            {
                if (components[i])
                {
                    components[i]->reserve(number_of_entities);
                }
            }

            entity_repository.reserve(number_of_entities);
        }

	    /**
	     * \brief Creates a new entity
	     * \return The newly created entity
	     */
	    entity create_entity()
        {
            return entity_repository.create();
        }

	    /**
	     * \brief Removes the entity and updates the systems and component array
	     * \param entity The entity that is being removed
	     */
	    void remove_entity(entity entity)
        {
            for (auto& component : components)
            {
                if (component)
                {
                    component->try_remove(entity);
                }
            }

            for (auto& system : systems)
            {
                system->on_entity_removed(entity);
            }

            entity_repository.remove(entity);
        }

	    /**
	     * \brief Checks if entity has component
	     * \tparam T Component type
	     * \param entity The entity that is being checked
	     * \return 
	     */
	    template<typename T>
        bool has_component(entity entity) const
        {
            check_component_type<T>();
            return entity_repository.get_entity(entity)[T::type];
        }

	    /**
	     * \brief Checks if entity has components
	     * \tparam Ts Component types
	     * \param entity The entity that is being checked
	     * \return 
	     */
	    template<typename ...Ts>
        bool has_components(entity entity) const
        {
            check_component_types<Ts...>();

            // Building up the components into a bitset
            auto requirements = std::bitset<component_count>();
            (requirements.set(Ts::type), ...);

            // Getting entity's components and comparing to see if the entity has the same components
            const auto entity_components = entity_repository.get_entity(entity);
            return (requirements & entity_components) == requirements;
        }

        template<typename ...Ts>
        bool has_components(entity entity, std::bitset<component_count> requirements) const
        {
            check_component_types<Ts...>();

            // Getting entity's components and comparing to see if the entity has the same components
            const auto entity_components = entity_repository.get_entity(entity);
            return (requirements & entity_components) != 0;
        }

        template<typename T>
        T& get_component(entity entity)
        {
            check_component_type<T>();
            return get_component_array<T>()->get(entity);
        }

        template<typename T>
        const T& get_component(entity entity) const
        {
            check_component_type<T>();
            return get_component_array<T>()->get(entity);
        }

        template<typename ...Ts>
        std::tuple<Ts&...> get_components(entity entity)
        {
            check_component_types<Ts...>();
            return std::tie(get_component_array<Ts>()->get(entity)...);
        }

        template<typename ...Ts>
        std::tuple<const Ts&...> get_components(entity entity) const
        {
            check_component_types<Ts...>();
            return std::tie(get_component_array<Ts>()->get(entity)...);
        }

        template<typename T, typename... Args>
        void add_component(entity entity, Args&&... args)
        {
            check_component_type<T>();
            get_component_array<T>()->add(entity, std::forward<Args>(args)...);

            const auto& entity_components = entity_repository.get_entity(entity);
            for (auto& system : systems)
            {
                system->on_entity_updated(entity, entity_components);
            }
        }

        template<typename T>
        void remove_component(entity entity)
        {
            check_component_type<T>();
            get_component_array<T>()->remove(entity);

            const auto& entity_components = entity_repository.get_entity(entity);
            for (auto& system : systems)
            {
                system->on_entity_updated(entity, entity_components);
            }
        }

        const int get_width() const
        {
            return width;
        }

        const int get_height() const
        {
            return height;
        }

        void update(float dt) const
        {
		    for (auto& system_update : systems_updates)
		    {
                system_update(dt);
		    }
	    }

        bool is_alive(entity entity) const
	    {
            return entity_repository.is_alive(entity);
	    }

        /*
         * SPATIAL GRID
         */
        void update_grid(ecs::entity entity, const SDL_FRect& new_rect_data)
	    {
            spatial_grid.update(entity, new_rect_data);
	    }

        void insert_to_grid(ecs::entity entity, const SDL_FRect& rect_data)
        {
            spatial_grid.insert(entity, rect_data);
        }

        void remove_from_grid(ecs::entity entity)
        {
            spatial_grid.remove(entity);
        }

		template<typename... Ts>
		std::unordered_set<entity> find_nearby_grid(const SDL_FRect& rect_data, const std::unordered_set<entity>& excluded_entities = {})
		{
		    std::bitset<component_count> include_all_components;
		    (include_all_components.set(Ts::type), ...);

		    return std::move(spatial_grid.find_nearby(rect_data, [&](ecs::entity entity)
		    {
                    return (excluded_entities.empty() || !excluded_entities.contains(entity))
		    				&& (!include_all_components.any()
								|| (include_all_components.any() && has_components(entity, include_all_components)));
		    }));
		}
        
    private:
        int width;
        int height;
        std::array<std::unique_ptr<base_component_array>, component_count> components{};
        entities::entity_repository<component_count> entity_repository{};
        std::vector<std::unique_ptr<system<component_count, system_count>>> systems{};
        std::vector<update_func> systems_updates{};
		collision::spatial_grid<component_count, system_count> spatial_grid;

        template<typename T>
        void check_component_type() const
        {
            static_assert(T::type < component_count);
        }

        template<typename ...Ts>
        void check_component_types() const
        {
            (check_component_type<Ts>(), ...);
        }

        template<typename T>
        auto get_component_array()
        {
            return static_cast<component_array<T, component_count>*>(components[T::type].get());
        }
        template<typename T>
        auto get_component_array() const
        {
            return static_cast<const component_array<T, component_count>*>(components[T::type].get());
        }
    };
}

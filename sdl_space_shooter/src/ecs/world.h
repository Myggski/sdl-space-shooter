#pragma once

#include "base_component_repository.h"
#include "component_repository.h"
#include "entity_repository.h"
#include "system.h"

namespace ecs
{
    constexpr auto EntityCount = 1024;
    constexpr auto ComponentCount = 32;
    constexpr auto SystemCount = 8;

    template<std::size_t component_count, std::size_t system_count>
    class world
    {
    public:
        template<typename T>
        void register_component()
        {
            check_component_type<T>();
            components[T::type] = std::make_unique<ecs::component_repository<T, component_count, system_count>>(
                entities.get_entity_to_component(T::type), entities.get_entity_to_bitset());
        }
        template<typename T, typename ...Args>
        T* create_system(Args&& ...args)
        {
            auto type = systems.size();
            auto& system = systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
            system->setup(type, &entities.get_entity_to_managed_entity(type));
            return static_cast<T*>(system.get());
        }
        void reserve(std::size_t size)
        {
            for (size_t i = 0; i < component_count; ++i)
            {
                if (components[i])
                {
                    components[i]->reserve(size);
                }
            }

            entities.reserve(size);
        }
        ecs::entity create_entity()
        {
            return entities.create();
        }
        void remove_entity(ecs::entity entity)
        {
            entities.remove(entity);
        }
        template<typename T>
        bool has_component(ecs::entity entity) const
        {
            check_component_type<T>();
            return entities.get_bitset(entity)[T::type];
        }
        template<typename ...Ts>
        bool has_components(ecs::entity entity) const
        {
            check_component_types<Ts...>();
            auto requirements = std::bitset<component_count>();
            (requirements.set(Ts::type), ...);
            return (requirements & entities.get_bitset(entity)) == requirements;
        }
        template<typename T>
        T& get_component(ecs::entity entity)
        {
            check_component_type<T>();
            return get_component_repository<T>()->get(entity);
        }
        template<typename T>
        const T& get_component(ecs::entity entity) const
        {
            check_component_type<T>();
            return get_component_repository<T>()->get(entity);
        }
        template<typename ...Ts>
        std::tuple<Ts&...> get_components(ecs::entity entity)
        {
            check_component_types<Ts...>();
            return std::tie(get_component_repository<Ts>()->get(entity)...);
        }
        template<typename ...Ts>
        std::tuple<const Ts&...> get_components(ecs::entity entity) const
        {
            check_component_types<Ts...>();
            return std::tie(get_component_repository<Ts>()->get(entity)...);
        }
        template<typename T, typename... Args>
        void add_component(ecs::entity entity, Args&&... args)
        {
            check_component_type<T>();
            get_component_repository<T>()->add(entity, std::forward<Args>(args)...);
            
            const auto& bitset = entities.get_bitset(entity);
            for (auto& system : systems)
            {
                system->on_entity_updated(entity, bitset);
            }
        }
        template<typename T>
        void remove_component(ecs::entity entity)
        {
            check_component_type<T>();
            get_component_repository<T>()->remove(entity);
            // Send message to systems
            const auto& bitset = entities.get_bitset(entity);
            for (auto& system : systems)
            {
                system->on_entity_updated(entity, bitset);
            }
        }
        template<typename T>
        ecs::entity get_owner(const T& component) const
        {
            check_component_type<T>();
            return get_component_repository<T>()->getOwner(component);
        }

    private:
        std::array<std::unique_ptr<ecs::base_component_repository>, component_count> components;
        entity_repository<component_count, system_count> entities;
        std::vector<std::unique_ptr<system<component_count, system_count>>> systems;

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
        auto get_component_repository()
        {
            return static_cast<ecs::component_repository<T, component_count, system_count>*>(components[T::type].get());
        }
        template<typename T>
        auto get_component_repository() const
        {
            return static_cast<const ecs::component_repository<T, component_count, system_count>*>(components[T::type].get());
        }
    };
}
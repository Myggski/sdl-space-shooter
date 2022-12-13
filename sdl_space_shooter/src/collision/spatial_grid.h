#pragma once

#include <string>
#include <unordered_map>
#include <iterator>
#include <set>
#include "SDL.h"
#include "ecs/entity.h"

namespace ecs
{
	template<std::size_t component_count, std::size_t system_count>
	class world;
}

namespace collision
{
	static constexpr int CELL_SIZE = 128;

	template<std::size_t component_count, std::size_t system_count>
	class spatial_grid final
	{
		friend ::ecs::world<component_count, system_count>;
	public:
		spatial_grid(const ::ecs::world<component_count, system_count>& world) : world(world)
		{
			entities.reserve(10000);
		}

		std::set<ecs::entity> find_nearby(const SDL_FRect& entity_data, const std::bitset<component_count>& excluded_components, const std::set<ecs::entity>& excluded_entities)
		{
			const int min_x = static_cast<int>(std::floor((entity_data.x - (entity_data.w * 0.5f)) / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((entity_data.x + (entity_data.w * 0.5f)) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor((entity_data.y - (entity_data.h * 0.5f)) / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((entity_data.y + (entity_data.h * 0.5f)) / CELL_SIZE));

			std::set<ecs::entity> nearby_entities;

			for (int x = min_x; x <= max_x; ++x)
			{
				for (int y = min_y; y <= max_y; ++y)
				{
					auto key = get_key(x, y);

					if (!entities[key].empty())
					{
						for (ecs::entity entity : entities[key])
						{
							if ((excluded_entities.empty() || !excluded_entities.contains(entity)) && (!excluded_components.any() || (excluded_components.any() && !world.has_components(entity, excluded_components))))
							{
								nearby_entities.insert(entity);
							}
						}
					}
				}
			}

			return std::move(nearby_entities);
		}
	private:
		std::unordered_map<std::string, std::set<ecs::entity>> entities;
		const ecs::world<component_count, system_count>& world;

		void update(const ecs::entity entity, const SDL_FRect& old_rect, const SDL_FRect& new_rect)
		{
			remove(entity, old_rect);
			insert(entity, new_rect);
		}

		bool remove(const ecs::entity entity, const SDL_FRect& entity_data)
		{
			// Compute the coordinates of the cell that the entity belongs to
			const int x = static_cast<int>(std::floor((entity_data.x + (entity_data.w * 0.5f)) / CELL_SIZE));
			const int y = static_cast<int>(std::floor((entity_data.y + (entity_data.h * 0.5f)) / CELL_SIZE));
			const auto key = get_key(x, y);

			if (!entities[key].empty())
			{
				entities[key].erase(entity);

				if (entities.contains(key) && entities[key].empty())
				{
					entities.erase(key);
				}

				return true;
			}

			return false;
		}
		void insert(const ecs::entity entity, const SDL_FRect& entity_data)
		{
			// Compute the coordinates of the cell that the entity belongs to
			const int x = static_cast<int>(std::floor((entity_data.x + (entity_data.w * 0.5f)) / CELL_SIZE));
			const int y = static_cast<int>(std::floor((entity_data.y + (entity_data.h * 0.5f)) / CELL_SIZE));
			const auto key = get_key(x, y);

			entities[key].insert(entity);
		}

		static std::string get_key(int x, int y)
		{
			char buffer[15];
			std::snprintf(buffer, sizeof(buffer), "%d,%d", x, y);
			return std::string(buffer);
		}
	};
}

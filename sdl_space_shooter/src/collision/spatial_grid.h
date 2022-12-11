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
	static constexpr int CELL_SIZE = 64;

	template<std::size_t component_count, std::size_t system_count>
	class spatial_grid final
	{
		friend ::ecs::world<component_count, system_count>;
	public:
		spatial_grid()
		{
			entities.reserve((1280 / 64) * (720 / 64));
		}

		std::vector<ecs::entity> find_nearby(const SDL_FRect& entity_data, std::set<ecs::entity> exclude)
		{
			const int min_x = static_cast<int>(std::floor(entity_data.x / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((entity_data.x + entity_data.w) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor(entity_data.y / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((entity_data.y + entity_data.h) / CELL_SIZE));

			std::vector<ecs::entity> nearby_entities;
			for (int x = min_x; x <= max_x; ++x)
			{
				for (int y = min_y; y <= max_y; ++y)
				{
					auto key = get_key(x, y);

					if (!entities[key].empty())
					{
						std::copy_if(
							entities[key].begin(),
							entities[key].end(),
							std::back_inserter(nearby_entities),
							[exclude](const auto& entity) { return exclude.empty() || !exclude.contains(entity); }
						);
					}
				}
			}
			return nearby_entities;
		}
	private:
		std::unordered_map<std::string, std::set<ecs::entity>> entities;

		void update(const ecs::entity entity, const SDL_FRect& old_rect, const SDL_FRect& new_rect)
		{
			remove(entity, old_rect);
			insert(entity, new_rect);
		}

		bool remove(const ecs::entity entity, const SDL_FRect& entity_data)
		{
			// Compute the coordinates of the cell that the entity belongs to
			const int x = static_cast<int>(std::floor(entity_data.x / CELL_SIZE));
			const int y = static_cast<int>(std::floor(entity_data.y / CELL_SIZE));
			const auto key = get_key(x, y);

			if (!entities[key].empty())
			{
				entities[key].erase(entity);

				if (entities[key].empty())
				{
					entities.erase(key);
					return true;
				}
			}

			return false;
		}
		void insert(const ecs::entity entity, const SDL_FRect& entity_data)
		{
			// Compute the coordinates of the cell that the entity belongs to
			const int x = static_cast<int>(std::floor(entity_data.x / CELL_SIZE));
			const int y = static_cast<int>(std::floor(entity_data.y / CELL_SIZE));
			const auto key = get_key(x, y);

			entities[key].insert(entity);
		}

		std::string get_key(int x, int y) const
		{
			return std::to_string(x) + "," + std::to_string(y);
		}
	};
}

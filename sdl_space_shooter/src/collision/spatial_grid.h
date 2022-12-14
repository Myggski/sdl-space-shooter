#pragma once

#include <string>
#include <unordered_map>
#include <iterator>
#include "ecs/entity.h"
#include "ecs/setup_data.h"

namespace ecs
{
	template<std::size_t component_count, std::size_t system_count>
	class world;
}

namespace collision
{
	struct spatial_data
	{
		int min_x;
		int max_x;
		int min_y;
		int max_y;
	};

	static constexpr int CELL_SIZE = 128;

	template<std::size_t component_count, std::size_t system_count>
	class spatial_grid final
	{
		friend ::ecs::world<component_count, system_count>;
	public:
		spatial_grid()
		{
			entities.reserve(1280 / CELL_SIZE * 720 / CELL_SIZE);
			entity_to_coordinates.reserve(ecs::MAX_ENTITIES);

			for (int x = 0; x < 1280 / CELL_SIZE; x++)
			{
				for (int y = 0; y < 720 / CELL_SIZE; y++)
				{
					entities[get_key(x, y)].reserve(ecs::MAX_ENTITIES);
				}
			}
		}

		std::unordered_set<ecs::entity> find_nearby(const SDL_FRect& entity_data, std::function<bool(ecs::entity)> include_func)
		{
			const int min_x = static_cast<int>(std::floor((entity_data.x - (entity_data.w * 0.5f)) / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((entity_data.x + (entity_data.w * 0.5f)) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor((entity_data.y - (entity_data.h * 0.5f)) / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((entity_data.y + (entity_data.h * 0.5f)) / CELL_SIZE));

			std::unordered_set<ecs::entity> nearby_entities;

			for (int x = min_x; x <= max_x; ++x)
			{
				for (int y = min_y; y <= max_y; ++y)
				{
					auto key = get_key(x, y);

					if (!entities[key].empty())
					{
						std::ranges::copy_if(entities[key], std::inserter(nearby_entities, nearby_entities.begin()), include_func);
					}
				}
			}

			return std::move(nearby_entities);
		}
	private:
		std::unordered_map<size_t, std::unordered_set<ecs::entity>> entities;
		std::unordered_map<ecs::entity, spatial_data> entity_to_coordinates;

		void update(const ecs::entity entity, const SDL_FRect& new_rect)
		{
			remove(entity);
			insert(entity, new_rect);
		}

		void remove(const ecs::entity entity)
		{
			const auto& coordinates = entity_to_coordinates[entity];

			for (int x = coordinates.min_x; x <= coordinates.max_x; ++x)
			{
				for (int y = coordinates.min_y; y <= coordinates.max_y; ++y)
				{
					const auto key = get_key(x, y);

					if (!entities[key].empty())
					{
						entities[key].erase(entity);
					}
				}
			}

			entity_to_coordinates.erase(entity);
			
		}

		void insert(const ecs::entity entity, const SDL_FRect& entity_data)
		{
			const int min_x = static_cast<int>(std::floor((entity_data.x - (entity_data.w * 0.5f)) / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((entity_data.x + (entity_data.w * 0.5f)) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor((entity_data.y - (entity_data.h * 0.5f)) / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((entity_data.y + (entity_data.h * 0.5f)) / CELL_SIZE));
			entity_to_coordinates[entity] = {
				min_x,
				max_x,
				min_y,
				max_y
			};

			for (int x = min_x; x <= max_x; ++x)
			{
				for (int y = min_y; y <= max_y; ++y)
				{
					entities[get_key(x, y)].emplace(entity);
				}
			}
		}

		static size_t get_key(int x, int y)
		{
			char buffer[15];
			std::snprintf(buffer, sizeof(buffer), "%d,%d", x, y);
			std::hash<std::string> hasher;
			auto hashed = hasher(buffer);
			return hashed;
		}
	};
}

#pragma once

#include <string>
#include <unordered_map>
#include <iterator>
#include "ecs/entity.h"

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
		spatial_grid(size_t max_entities, float grid_width, float grid_height)
		{
			entities.reserve(grid_width / CELL_SIZE * grid_height / CELL_SIZE);
			entity_to_coordinates.reserve(max_entities);

			// Calculate the number of cells in the x and y directions
			const int max_cells_x = grid_width / CELL_SIZE;
			const int max_cells_y = grid_height / CELL_SIZE;
			const int cell_size = max_cells_x * max_cells_y;

			for (int i = 0; i < cell_size; i++)
			{
				entities[get_key(i / max_cells_y, i % max_cells_y)].reserve(max_entities);
			}
		}

		std::unordered_set<ecs::entity> find_nearby(const SDL_FRect& entity_data, std::function<bool(ecs::entity)> include_func)
		{
			std::unordered_set<ecs::entity> nearby_entities;
			const int min_x = static_cast<int>(std::floor((entity_data.x - (entity_data.w * 0.5f)) / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((entity_data.x + (entity_data.w * 0.5f)) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor((entity_data.y - (entity_data.h * 0.5f)) / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((entity_data.y + (entity_data.h * 0.5f)) / CELL_SIZE));
			const int cell_size = (max_x - min_x + 1) * (max_y - min_y + 1);
			

			for (int i = 0; i < cell_size; i++)
			{
				const int x = min_x + i / (max_y - min_y + 1);
				const int y = min_y + i % (max_y - min_y + 1);
				const auto key = get_key(x, y);

				if (!entities[key].empty())
				{
					std::ranges::copy_if(entities[key], std::inserter(nearby_entities, nearby_entities.begin()), include_func);
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
			const auto cell_size = (coordinates.max_x - coordinates.min_x + 1) * (coordinates.max_y - coordinates.min_y + 1);

			for (int i = 0; i < cell_size; ++i)
			{
				const int x = coordinates.min_x + i / (coordinates.max_y - coordinates.min_y + 1);
				const int y = coordinates.min_y + i % (coordinates.max_y - coordinates.min_y + 1);
				const auto key = get_key(x, y);

				if (!entities[key].empty())
				{
					entities[key].erase(entity);
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
			const int cell_size = (max_x - min_x + 1) * (max_y - min_y + 1);

			for (int i = 0; i < cell_size; ++i)
			{
				const int x = min_x + i / (max_y - min_y + 1);
				const int y = min_y + i % (max_y - min_y + 1);

				entities[get_key(x, y)].emplace(entity);
			}

			entity_to_coordinates[entity] = {
				min_x,
				max_x,
				min_y,
				max_y
			};
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

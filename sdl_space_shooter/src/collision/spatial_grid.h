#pragma once

#include <string>
#include <unordered_map>
#include <iterator>
#include "ecs/entity.h"

#include "spatial_data_structure.h"
#include "utils/key_generator.h"

namespace collision
{
	static constexpr int CELL_SIZE = 128;

	template<typename T>
	class spatial_grid final : public spatial_data_structure<T>
	{
	public:
		spatial_grid(size_t max_entities, float grid_width, float grid_height)
		{
			entities.reserve(grid_width / CELL_SIZE * grid_height / CELL_SIZE);
			entity_to_coordinates.reserve(max_entities);

			const int max_cells_x = grid_width / CELL_SIZE;
			const int max_cells_y = grid_height / CELL_SIZE;
			const int cell_size = max_cells_x * max_cells_y;

			for (int i = 0; i < cell_size; i++)
			{
				entities[utils::get_key(i / max_cells_y, i % max_cells_y)].reserve(max_entities);
			}
		}

		std::unordered_set<T> find_nearby(const SDL_FRect& entity_data, std::function<bool(T)> include_func) override
		{
			std::unordered_set<T> nearby_entities;
			const int min_x = static_cast<int>(std::floor((entity_data.x - (entity_data.w * 0.5f)) / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((entity_data.x + (entity_data.w * 0.5f)) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor((entity_data.y - (entity_data.h * 0.5f)) / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((entity_data.y + (entity_data.h * 0.5f)) / CELL_SIZE));
			const int cell_size = (max_x - min_x + 1) * (max_y - min_y + 1);

			for (int i = 0; i < cell_size; i++)
			{
				const int x = min_x + i / (max_y - min_y + 1);
				const int y = min_y + i % (max_y - min_y + 1);
				const auto key = utils::get_key(x, y);

				if (!entities[key].empty())
				{
					std::ranges::copy_if(entities[key], std::inserter(nearby_entities, nearby_entities.begin()), include_func);
				}
			}

			return std::move(nearby_entities);
		}

		void update(const T& data, const SDL_FRect& new_rect) override
		{
			remove(data);
			insert(data, new_rect);
		}

		bool remove(const T& data) override
		{
			if (!entity_to_coordinates.contains(data))
			{
				return false;
			}

			const auto& rect = entity_to_coordinates[data];
			const int min_x = static_cast<int>(std::floor((rect.x - (rect.w * 0.5f)) / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((rect.x + (rect.w * 0.5f)) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor((rect.y - (rect.h * 0.5f)) / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((rect.y + (rect.h * 0.5f)) / CELL_SIZE));

			for (int x = min_x; x < max_x; ++x)
			{
				for (int y = min_y; y < max_y; ++y)
				{
					const auto key = utils::get_key(x, y);

					if (!entities[key].empty())
					{
						entities[key].erase(data);
					}
				}
			}

			entity_to_coordinates.erase(data);

			return true;
		}

		void insert(const T data, const SDL_FRect& rect) override
		{
			const int min_x = static_cast<int>(std::floor((rect.x - (rect.w * 0.5f)) / CELL_SIZE));
			const int max_x = static_cast<int>(std::ceil((rect.x + (rect.w * 0.5f)) / CELL_SIZE));
			const int min_y = static_cast<int>(std::floor((rect.y - (rect.h * 0.5f)) / CELL_SIZE));
			const int max_y = static_cast<int>(std::ceil((rect.y + (rect.h * 0.5f)) / CELL_SIZE));

			for (int x = min_x; x < max_x; ++x)
			{
				for (int y = min_y; y < max_y; ++y)
				{
					entities[utils::get_key(x, y)].emplace(data);
				}
			}

			entity_to_coordinates[data] = rect;
		}

		void clear() override
		{
			entities.clear();
			entity_to_coordinates.clear();
		}
		
	private:
		std::unordered_map<size_t, std::unordered_set<T>> entities;
		std::unordered_map<T, SDL_FRect> entity_to_coordinates;
	};
}

#include "pch.h"

#include "wave_function_collapse.h"
#include "image_scan.h"
#include "utils/random.h"
#include "hex_color_utils.h"
#include "tile_factory.h"
#include "state/commands/remove_entropy_command.h"
#include "utils/key_generator.h"
#include "utils/math.h"

static SDL_Point get_direction(edge_direction_types edge)
{
	switch (edge)
	{
	case edge_direction_types::top:
		return { 0, -1 };
	case edge_direction_types::right:
		return { 1, 0 };
	case edge_direction_types::bottom:
		return { 0, 1 };
	case edge_direction_types::left:
	default:
		return { -1, 0 };
	}
}

wave_function_collapse::wave_function_collapse(int width, int height, int tile_size, tile_types tile_type)
	: number_of_sockets(tile_factory::get_number_of_sockets(tile_type)),
	  number_of_failures(0),
	  tile_size(tile_size),
	  start_position(),
	  latest_collapse_position(),
	  state(width * height * 4)
{
	tiles = tile_factory::create(tile_type);

	// Fills the grid with entropy
	grid_width = static_cast<int>(ceil(static_cast<float>(width) / tile_size));
	grid_height = static_cast<int>(ceil(static_cast<float>(height) / tile_size));

	grid.resize(grid_width);
	entropy_weight.reserve(grid_width * grid_height);

	for (int x = 0; x < grid_width; ++x)
	{
		grid[x].resize(grid_height);

		for (int y = 0; y < grid_height; ++y)
		{
			const auto position = SDL_Point{x, y};

			grid[x][y] = wave_cell{
				tiles,
				position,
				false,
			};
		}
	}
}

bool wave_function_collapse::run()
{
	// Algorithm is done
	if (!state.empty() && entropy_weight.empty())
	{
		return true;
	}

	auto& next_cell_to_collapse = get_weight_wave_cell();

	try
	{
		// Can throw exception, if there's not enough valid entropy left
		collapse_wave_cell(next_cell_to_collapse);

		std::unordered_set<wave_cell, wave_cell_hash> visited;
		update_neighbors(next_cell_to_collapse, visited);
	}
	catch (const std::exception&)
	{
		state.rewind(next_cell_to_collapse.get_position());
	}
	
	return false;
}

void wave_function_collapse::draw(SDL_Renderer* renderer, application::texture_manager& texture_manager) const
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < grid_width * grid_height; ++i)
	{
		x = i % grid_width;
		if (i % grid_width == 0 && i > 0)
		{
			y++;
		}

		if (!grid[x][y].get_entropy().empty())
		{
			auto entropy = grid[x][y].get_entropy();

			for (const auto& current_entropy : entropy)
			{
				auto image = texture_manager.get_image(current_entropy.file_path);
				Uint8 alpha = 255 / entropy.size();
				SDL_SetTextureAlphaMod(image, alpha);
				SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
				application::texture_manager::draw_texture(renderer, image, { x * tile_size, y * tile_size, tile_size, tile_size }, current_entropy.rotations_to_degrees());
			}
		}
	}
}

void wave_function_collapse::clear()
{
	state.clear();
	number_of_failures = 0;
}

void wave_function_collapse::update_neighbors(const wave_cell& collapsed_cell, std::unordered_set<wave_cell, wave_cell_hash>& visited)
{
	// If cell has already been visited, ignore
	if (visited.contains(collapsed_cell))
	{
		return;
	}

	visited.insert(collapsed_cell);
	latest_collapse_position = collapsed_cell.get_position();

	// Get collapsed position
	const auto cell_position = collapsed_cell.get_position();
	auto& collapsed_entropy = collapsed_cell.get_by_index(0);

	// For each corner of collapsed cell
	for (int edge_index = 0; edge_index < collapsed_entropy.edges.size(); ++edge_index)
	{
		// Get edge direction		
		const auto edge_direction = static_cast<edge_direction_types>(edge_index);
		const SDL_Point direction = get_direction(edge_direction);

		// Neighbor position
		const SDL_Point neighbor_position{
			static_cast<int>(cell_position.x) + direction.x,
		static_cast<int>(cell_position.y) + direction.y
		};

		// If neighbor is outside of screen, continue to the next edge
		if (neighbor_position.x < 0 || neighbor_position.y < 0 || neighbor_position.x >= grid_width || neighbor_position.y >= grid_height)
		{
			continue;
		}

		wave_cell& neighbor_cell = grid[neighbor_position.x][neighbor_position.y];

		if (neighbor_cell.is_collapsed())
		{
			continue;
		}

		tile_data_list entropy_to_remove;
		entropy_to_remove.reserve(neighbor_cell.get_entropy().size());

		// Check all neighbor entropy and remove the ones that's not fit anymore around neighbors
		for (const auto& neighbor_entropy : neighbor_cell.get_entropy())
		{
			if (!is_entropy_compatible(edge_direction, collapsed_entropy, neighbor_entropy))
			{
				entropy_to_remove.emplace(neighbor_entropy);
			}
		}

		if (!entropy_to_remove.empty())
		{
			state.execute_command({
					neighbor_cell,
					entropy_to_remove,
					entropy_weight
				});

			if (neighbor_cell.is_collapsed())
			{
				if (!is_compatible_with_neighbors(neighbor_position, neighbor_cell.get_by_index(0)))
				{
					state.rewind(collapsed_cell.get_position());
					return;
				}

				update_neighbors(neighbor_cell, visited);
			}
		}
	}
}

void wave_function_collapse::collapse_wave_cell(wave_cell& cell_to_collapse)
{
	const auto& entropy = cell_to_collapse.get_entropy();

	tile_data_list entropy_to_remove;
	entropy_to_remove.reserve(entropy.size());
	entropy_to_remove = get_invalid_entropy(cell_to_collapse);

	if (cell_to_collapse.get_entropy().size() == entropy_to_remove.size())
	{
		throw std::exception("Not enough valid entropy");
	}

	const auto it = entropy.begin();
	const int max = static_cast<int>(entropy.size() - 1);
	const tile_data* selected_entropy;

	while (true)
	{
		const int random_index = utils::get_random<int>(0, max);
		selected_entropy = &(*std::next(it, random_index));

		if (!entropy_to_remove.contains(*selected_entropy))
		{
			break;
		}
	}

	for (const auto& current_entropy : entropy)
	{
		if (current_entropy != *selected_entropy)
		{
			entropy_to_remove.emplace(current_entropy);
		}
	}

	state.execute_command({
		cell_to_collapse,
		entropy_to_remove,
		entropy_weight
		});
}

tile_data_list wave_function_collapse::get_invalid_entropy(const wave_cell& cell) const
{
	tile_data_list entropy_to_remove;
	entropy_to_remove.reserve(cell.get_entropy().size());

	for (auto& entropy : cell.get_entropy())
	{
		if (!is_compatible_with_neighbors(cell.get_position(), entropy))
		{
			entropy_to_remove.emplace(entropy);
		}
	}

	return entropy_to_remove;
}

bool wave_function_collapse::is_compatible_with_neighbors(const SDL_Point& cell_position, const tile_data& entropy) const
{
	for (int edge_index = 0; edge_index < entropy.edges.size(); ++edge_index)
	{
		bool is_valid = false;
		const auto direction_type = static_cast<edge_direction_types>(edge_index);
		const SDL_Point direction = get_direction(direction_type);
		const SDL_Point neighbor_position = {
			cell_position.x + direction.x,
			cell_position.y + direction.y
		};

		if (neighbor_position.x < 0 || neighbor_position.y < 0 || neighbor_position.x >= grid_width || neighbor_position.y >= grid_height)
		{
			continue;
		}

		wave_cell neighbor = grid[neighbor_position.x][neighbor_position.y];

		for (auto& neighbor_entropy : neighbor.get_entropy())
		{
			if (is_entropy_compatible(direction_type, entropy, neighbor_entropy))
			{
				is_valid = true;
				break;
			}
		}

		if (!is_valid)
		{
			return false;
		}
	}

	return true;
}

bool wave_function_collapse::is_entropy_compatible(edge_direction_types edge, const tile_data& a, const tile_data& b) const
{
	const auto edge_a = a.edges[static_cast<int>(edge)];
	const auto edge_b = b.edges[static_cast<int>(get_opposite_edge(edge))];
	const auto reverse_b = hex_color_utils::reverse_hex_string(edge_b, number_of_sockets);

	return edge_a == reverse_b;
}

edge_direction_types wave_function_collapse::get_opposite_edge(edge_direction_types edge)
{
	switch (edge)
	{
	case edge_direction_types::top:
		return edge_direction_types::bottom;
	case edge_direction_types::right:
		return edge_direction_types::left;
	case edge_direction_types::bottom:
		return edge_direction_types::top;
	case edge_direction_types::left:
	default:
		return edge_direction_types::right;
	}
}

wave_cell& wave_function_collapse::get_weight_wave_cell()
{
	if (!state.empty())
	{
		const auto& smallest_entropy = std::ranges::min_element(entropy_weight,
			[&](const auto& a, const auto& b)
			{
				const SDL_Point point_a = a.second.second;
				const SDL_Point point_b = b.second.second;

				const int weight_a = a.second.first + (utils::math::magnitude(point_a.x - start_position.x, point_a.y - start_position.y) * grid_width);
				const int weight_b = b.second.first + (utils::math::magnitude(point_b.x - start_position.x, point_b.y - start_position.y) * grid_width);

				return  weight_a < weight_b;
			}
		);

		if (smallest_entropy != entropy_weight.end())
		{
			return grid[smallest_entropy->second.second.x][smallest_entropy->second.second.y];
		}
	}

	const SDL_Point random_position{
	utils::get_random<int>(0, grid_width - 1),
		utils::get_random<int>(0, grid_height - 1)
	};

	if (state.empty())
	{
		start_position = random_position;
	}

	return grid[random_position.x][random_position.y];
}
#pragma once
#include <stack>

#include "edge_direction_types.h"
#include "tile_data.h"
#include "tile_factory.h"
#include "wave_cell.h"
#include "application/texture_manager.h"
#include "state/wave_state.h"

struct SDL_Point_Hash {
	std::size_t operator()(const SDL_Point& point) const {
		std::size_t hash_value = 0;
		hash_value = hash_value * 1337 + static_cast<int>(point.x);
		hash_value = hash_value * 1337 + static_cast<int>(point.y);

		return hash_value;
	}
};

struct wave_cell_hash {
	std::size_t operator()(const wave_cell& cell) const {
		std::size_t hash_value = 0;
		hash_value = hash_value * 42 + std::hash<int>()(cell.get_position().x);
		hash_value = hash_value * 42 + std::hash<int>()(cell.get_position().y);

		return hash_value;
	}
};

class wave_function_collapse
{
public:
	wave_function_collapse(int width, int height, int tile_size, tile_types tile_type);
	bool run();
	void draw(SDL_Renderer* renderer, application::texture_manager& texture_manager) const;
	void clear();

private:
	void update_neighbors(const wave_cell& collapsed_cell, std::unordered_set<wave_cell, wave_cell_hash>& visited);
	void collapse_wave_cell(wave_cell& cell_to_collapse);
	tile_data_list get_invalid_entropy(const wave_cell& cell) const;
	bool is_compatible_with_neighbors(const SDL_Point& cell_position, const tile_data& entropy) const;
	bool is_entropy_compatible(edge_direction_types edge, const tile_data& a, const tile_data& b) const;
	static edge_direction_types get_opposite_edge(edge_direction_types edge);
	wave_cell& get_weight_wave_cell();

private:
	int grid_width;
	int grid_height;
	int number_of_sockets;
	int number_of_failures;
	float tile_size;
	SDL_Point start_position;
	SDL_Point latest_collapse_position;
	wave_state state;
	std::vector<std::vector<wave_cell>> grid;
	std::unordered_map<size_t, std::pair<int, SDL_Point>> entropy_weight;
	tile_data_list tiles;
};

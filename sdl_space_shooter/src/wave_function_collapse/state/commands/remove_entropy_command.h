#pragma once

#include "wave_function_collapse/wave_cell.h"

class wave_state;

class remove_entropy_command final
{
public:
	remove_entropy_command(wave_cell& cell, tile_data_list removed_entropy, std::unordered_map<size_t, std::pair<int, SDL_Point>>& entropy_weight);

private:
	void execute() const;
	void undo() const;

private:
	wave_cell& cell;
	tile_data_list removed_entropy;
	std::unordered_map<size_t, std::pair<int, SDL_Point>>& entropy_weight;

	friend wave_state;
};

#pragma once

#include "tile_data.h"
#include "utils/sdl_operators.h"

struct wave_cell
{
public:
	wave_cell();
	wave_cell(tile_data_list possibilities, SDL_Point position, bool collapsed);
	bool operator==(const wave_cell& other) const;
	bool operator!=(const wave_cell& other) const;
	bool is_collapsed() const;
	tile_data_list get_entropy() const;
	const SDL_Point& get_position() const;
	const tile_data& get_by_index(int index) const;
	void add(const tile_data_list entropy_to_add);
	void remove(const tile_data_list entropy_to_remove);

private:
	bool collapsed;
	tile_data_list entropy;
	SDL_Point position;
};

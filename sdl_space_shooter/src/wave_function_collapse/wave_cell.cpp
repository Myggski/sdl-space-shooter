#include "pch.h"
#include "wave_cell.h"

wave_cell::wave_cell()
	: collapsed(false), entropy({}), position({ -9999, -9999 }) {}

wave_cell::wave_cell(tile_data_list possibilities, SDL_Point position, bool collapsed)
	: collapsed(collapsed), entropy(std::move(possibilities)), position(position) { }

bool wave_cell::operator==(const wave_cell& other) const
{
	if (position != other.position)
	{
		return false;
	}

	return true;
}

bool wave_cell::operator!=(const wave_cell& other) const {
	return !(*this == other);
}

bool wave_cell::is_collapsed() const
{
	return entropy.size() == 1;
}

tile_data_list wave_cell::get_entropy() const
{
	return entropy;
}

const SDL_Point& wave_cell::get_position() const
{
	return position;
}

const tile_data& wave_cell::get_by_index(int index) const
{
	auto it = entropy.begin();
	return *std::next(it, index);
}

void wave_cell::add(const tile_data_list entropy_to_add)
{
	entropy.insert(entropy_to_add.begin(), entropy_to_add.end());
}

void wave_cell::remove(const tile_data_list entropy_to_remove)
{
	for (auto& current_entropy : entropy_to_remove)
	{
		if (entropy.contains(current_entropy))
		{
			entropy.erase(current_entropy);
		}
	}
}
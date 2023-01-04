#include "pch.h"

#include "remove_entropy_command.h"
#include "utils/key_generator.h"
#include <utility>

remove_entropy_command::remove_entropy_command(wave_cell& cell, tile_data_list removed_entropy, std::unordered_map<size_t, std::pair<int, SDL_Point>>& entropy_weight)
	: cell(cell), removed_entropy(std::move(removed_entropy)), entropy_weight(entropy_weight) {}

void remove_entropy_command::execute() const
{
	const auto key = utils::get_key(cell.get_position());
	const int diff = cell.get_entropy().size() - removed_entropy.size();

	if (!entropy_weight.contains(key) && diff > 1)
	{
		entropy_weight.emplace(key, std::make_pair(removed_entropy.size(), cell.get_position()));
	} else if (diff <= 1)
	{
		entropy_weight.erase(key);
	} else
	{
		entropy_weight[key].first -= static_cast<int>(removed_entropy.size());
	}
	
	cell.remove(removed_entropy);
}

void remove_entropy_command::undo() const
{
	const auto key = utils::get_key(cell.get_position());

	if (!entropy_weight.contains(key))
	{
		entropy_weight.emplace(key, std::make_pair(removed_entropy.size(), cell.get_position()));
	}
	else {
		entropy_weight[key].first += static_cast<int>(removed_entropy.size());
	}

	cell.add(removed_entropy);
}

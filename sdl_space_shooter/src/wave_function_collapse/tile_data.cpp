#include "pch.h"

#include "tile_data.h"

#include <regex>

uint64_t tile_data::next_id = -1;

tile_data::tile_data() : unique_id(-1), file_path({}), number_of_rotations(0), number_of_sockets(0) { }

tile_data::tile_data(std::string file_path, int number_of_sockets)
	: unique_id(next_id++), file_path(std::move(file_path)), number_of_rotations(0), number_of_sockets(number_of_sockets)
{
	for (auto& edge : edges)
	{
		edge.resize(number_of_sockets);
	}
}

tile_data::tile_data(const tile_data& rhs)
    : file_path(rhs.file_path), edges(rhs.edges), number_of_rotations(rhs.number_of_rotations),
    number_of_sockets(rhs.number_of_sockets), unique_id(rhs.unique_id) { }

bool tile_data::operator==(const tile_data& rhs) const
{
    if (unique_id != rhs.unique_id)
    {
        return false;
    }

    if (file_path != rhs.file_path)
    {
        return false;
    }

    if (number_of_rotations != rhs.number_of_rotations)
    {
        return false;
    }

    if (number_of_sockets != rhs.number_of_sockets)
    {
        return false;
    }

    return true;
}

bool tile_data::operator!=(const tile_data& rhs) const
{
    return !operator==(rhs);
}


tile_data& tile_data::operator=(const tile_data& rhs)
{
    if (this != &rhs)
    {
        file_path = rhs.file_path;
        number_of_rotations = rhs.number_of_rotations;
        number_of_sockets = rhs.number_of_sockets;
        edges = rhs.edges;
    }

    return *this;
}

/**
 * \brief Rotates a tile depending on number of rotations
 * \param number_of_rotations 1 rotation = 90 degrees
 * \return A rotated tile
 */
tile_data tile_data::rotate_tile(int rotations)
{
    tile_data rotated_tile{ file_path, number_of_sockets };

    std::array<std::string, 4> rotated_edges;

    for (size_t i = 0; i < edges.size(); i++)
    {
        rotated_edges[i] = edges[(i - rotations + edges.size()) % edges.size()];
    }

    rotated_tile.edges = rotated_edges;
    rotated_tile.number_of_rotations = number_of_rotations + rotations;

    return rotated_tile;
}

std::string tile_data::get_file_name() const
{
    std::regex file_regex("([^/]+\\.[^/.]+)$");
    std::smatch file_name_match;

    std::regex_search(file_path, file_name_match, file_regex);

    const std::string file_name = file_name_match.size() > 1
        ? file_name_match[1]
        : std::string{};

    assert(!file_name.empty());

    return file_name;
}


float tile_data::rotations_to_degrees() const
{
    return static_cast<float>(number_of_rotations) * 90.f;
}

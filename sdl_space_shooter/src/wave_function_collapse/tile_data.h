#pragma once

#include <array>

struct tile_data
{
public:
	~tile_data() = default;
    tile_data();
    tile_data(std::string file_name, int number_of_sockets);
    tile_data(const tile_data& rhs);

    tile_data rotate_tile(int number_of_rotations);
    float rotations_to_degrees() const;
	std::string get_file_name() const;
    bool operator==(const tile_data& rhs) const;
    bool operator!=(const tile_data& rhs) const;
    tile_data& operator=(const tile_data& rhs);

public:
	std::string file_path;
    std::array<std::string, 4> edges;
    int number_of_rotations;
    int number_of_sockets;
	size_t unique_id;

private:
	static size_t next_id;

};

namespace utils
{
	template <typename T>
	void hash_combine(size_t& seed, T value)
	{
		seed ^= std::hash<T>()(value) ^ seed;
	}

	struct tile_data_hash
	{
		size_t operator()(const tile_data& tile_data) const
		{
			std::hash<std::string> hasher;
			size_t file_name_hash = hasher(tile_data.file_path);
			size_t rotations_hash = std::hash<int>{}(tile_data.number_of_rotations);
			size_t unique_id_hash = std::hash<int>{}(tile_data.unique_id);

			size_t hash = 42;
			utils::hash_combine<size_t>(hash, file_name_hash);
			utils::hash_combine<size_t>(hash, rotations_hash);
			utils::hash_combine<size_t>(hash, unique_id_hash);
			return hash;
		}
	};

	struct tile_data_equal
	{
		bool operator()(const tile_data& lhs, const tile_data& rhs) const
		{
			if (lhs.unique_id != rhs.unique_id)
			{
				return false;
			}

			if (lhs.file_path != rhs.file_path)
			{
				return false;
			}

			if (lhs.number_of_rotations != rhs.number_of_rotations)
			{
				return false;
			}

			return true;
		}
	};
}

using tile_data_list = std::unordered_set<tile_data, utils::tile_data_hash, utils::tile_data_equal>;
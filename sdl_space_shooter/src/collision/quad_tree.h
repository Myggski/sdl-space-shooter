#pragma once

#include <vector>
#include <unordered_map>

#include "aabb.h"
#include "spatial_data_structure.h"

namespace collision
{
	template<typename T>
	struct spatial_data
	{
	public:
		T data;
		SDL_FRect rect;
	};

	template<typename T>
	class quad_tree final : public spatial_data_structure<T>
	{
	public:
		quad_tree(struct SDL_FRect boundary, int max_nodes)
			: boundary(boundary), max_nodes(max_nodes)
		{
			data_list.reserve(max_nodes);
			data_to_data_index.reserve(max_nodes);
		}

		std::unordered_set<T> find_nearby(const SDL_FRect& rect, std::function<bool(T)> include_func) override
		{
			std::unordered_set<T> nearby_entities;
			nearby_entities.reserve(max_nodes);

			if (is_out_of_bound(boundary, rect))
			{
				return nearby_entities;
			}

			for (const auto& data_item : data_list) {
				if (collision::is_colliding(data_item.rect, rect) && include_func(data_item.data))
				{
					nearby_entities.emplace(data_item.data);
				}
			}

			auto child_index = get_child_index(boundary, rect);

			if (child_index == -1 || !children[child_index])
			{
				return nearby_entities;
			}

			quad_tree<T>& child = *children[child_index].get();
			std::unordered_set<T> found_data = child.find_nearby(rect, include_func);
			nearby_entities.insert(found_data.begin(), found_data.end());

			return nearby_entities;
		}

		void update(const T& data, const SDL_FRect& new_rect) override
		{
			remove(data);
			insert(data, new_rect);
		}

		bool remove(const T& data) override
		{
			if (!data_to_data_index.contains(data))
			{
				return try_remove_from_children(data);
			}

			return try_remove_from_self(data);
		}

		void insert(const T data, const SDL_FRect& data_rect) override
		{
			spatial_data<T> quad_data { data, data_rect };

			if (!is_out_of_bound(boundary, data_rect) && data_list.size() < max_nodes) {
				data_to_data_index[data] = data_list.size();
				data_list.emplace_back(quad_data);

				return;
			}

			if (!children[0]) {
				const float half_width = boundary.w / 2.f;
				const float half_height = boundary.h / 2.f;

				children[0] = std::make_unique<quad_tree<T>>(SDL_FRect{boundary.x, boundary.y, half_width, half_height}, max_nodes);
				children[1] = std::make_unique<quad_tree<T>>(SDL_FRect{ boundary.x + half_width, boundary.y, half_width, half_height }, max_nodes);
				children[2] = std::make_unique<quad_tree<T>>(SDL_FRect{ boundary.x, boundary.y + half_height, half_width, half_height }, max_nodes);
				children[3] = std::make_unique<quad_tree<T>>(SDL_FRect{ boundary.x + half_width, boundary.y + half_height, half_width, half_height }, max_nodes);
			}

			for (const auto& child : children)
			{
				if (child && !is_out_of_bound(boundary, data_rect))
				{
					child->insert(data, data_rect);
				}
			}
		}

		void clear() override
		{
			data_list.clear();
			data_to_data_index.clear();
			std::fill(children.begin(), children.end(), nullptr);
		}

	private:
		bool try_remove_from_children(const T& data)
		{
			for (auto& child : children)
			{
				if (child && child->remove(data))
				{
					if (child->is_empty())
					{
						child = nullptr;
					}

					return true;
				}
			}

			return false;
		}

		bool try_remove_from_self(const T& data)
		{
			const auto index_to_remove = data_to_data_index[data];

			if (data_list.empty() || index_to_remove < 0)
			{
				return false;
			}

			data_to_data_index[data_list.back().data] = index_to_remove;
			data_to_data_index[data] = -1;
			data_list[index_to_remove] = data_list.back();
			data_list.pop_back();

			if (is_empty())
			{
				std::fill(children.begin(), children.end(), nullptr);
			}

			return true;
		}

		bool is_empty() const
		{
			if (!data_list.empty())
			{
				return false;
			}

			for (const auto& child : children)
			{
				if (child && !child->is_empty())
				{
					return false;
				}
			}

			return true;
		}
		static bool is_out_of_bound(const SDL_FRect& boundary, const SDL_FRect& rect)
		{
			return rect.x + rect.w < boundary.x
				|| rect.x > boundary.x + boundary.w
				|| rect.y + rect.h < boundary.y
				|| rect.y > boundary.y + boundary.h;
		}

		int get_child_index(const SDL_FRect& boundary, const SDL_FRect& rect)
		{
			const float boundary_center_x = boundary.x + boundary.w / 2.f;
			const float boundary_center_y = boundary.y + boundary.h / 2.f;
			int index;

			if (rect.x + rect.w < boundary_center_x) {
				index = rect.y + rect.h < boundary_center_y ? 0 : 2;
			}
			else {
				index = rect.y + rect.h < boundary_center_y ? 1 : 3;
			}

			if (!children[index] || is_out_of_bound(children[index]->boundary, rect)) {
				index = -1;
			}

			return index;
		}

	private:
		std::array<std::unique_ptr<quad_tree<T>>, 4> children;
		SDL_FRect boundary;
		int max_nodes;
		std::vector<spatial_data<T>> data_list;
		std::unordered_map<T, int> data_to_data_index;
	};
}

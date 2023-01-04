#pragma once

namespace collision
{
	template<typename T>
	class spatial_data_structure
	{
	public:
		virtual ~spatial_data_structure() = default;

		virtual void insert(const T data, const SDL_FRect& rect) = 0;
		virtual std::unordered_set<T> find_nearby(const SDL_FRect& rect, std::function<bool(T)> include_func) = 0;
		virtual bool remove(const T& data) = 0;
		virtual void update(const T& data, const SDL_FRect& new_rect) = 0;
		virtual void clear() = 0;
	};
}
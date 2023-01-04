#pragma once

namespace application
{
	class font_manager
	{
	public:
		static font_manager& get_instance();

		static bool init();
		bool load(const std::string& name, const std::string& file, int size);
		void draw(SDL_Renderer* renderer, const std::string& font_name, const std::string& text, float x, float y) const;
		void clear();

		TTF_Font* get(const std::string& name) const;

	private:
		font_manager() = default;
		~font_manager() = default;

	private:
		std::unordered_map<std::string, TTF_Font*> fonts;
	};
}

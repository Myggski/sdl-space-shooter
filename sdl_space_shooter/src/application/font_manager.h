#pragma once

class font_manager
{
public:
	static font_manager& get_instance();
	bool init();
	bool load(const std::string& name, const std::string& file, int size);
	void draw(SDL_Renderer* renderer, const std::string& font_name, const std::string& text, float x, float y);

	TTF_Font* get(const std::string& name);
	void clear();
private:
	font_manager() = default;
	std::unordered_map<std::string, TTF_Font*> fonts;
};

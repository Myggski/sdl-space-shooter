#pragma once

class hex_color_utils
{
public:
	static std::string reverse_hex_string(const std::string& socket_in_hex, int number_of_sockets);
	static std::string rgb_to_hex(const SDL_Color& color);
	static std::vector<std::string> split_colors(const std::string& colors, int number_of_sockets);
	static std::string join_colors(const std::vector<std::string>& colors, int number_of_sockets);

};

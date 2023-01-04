#include "pch.h"
#include "hex_color_utils.h"

std::string hex_color_utils::reverse_hex_string(const std::string& socket_in_hex, int number_of_sockets)
{
	auto colors = split_colors(socket_in_hex, number_of_sockets);
	std::ranges::reverse(colors);

	return join_colors(colors, number_of_sockets);
}

std::string hex_color_utils::rgb_to_hex(const SDL_Color& color)
{
    char hex_string[13];
    sprintf_s(hex_string, "#%02X%02X%02X", color.r, color.g, color.b);

    return hex_string;
}

std::vector<std::string> hex_color_utils::split_colors(const std::string& colors, int number_of_sockets)
{
	std::vector<std::string> split_colors;
    split_colors.reserve(number_of_sockets);

    std::stringstream ss(colors);
    std::string hex_color;

    while (std::getline(ss, hex_color, '#')) {
        if (!hex_color.empty()) {
            split_colors.emplace_back(hex_color);
        }
    }

    return split_colors;
}

std::string hex_color_utils::join_colors(const std::vector<std::string>& colors, int number_of_sockets)
{
	std::string joined_colors;
    joined_colors.reserve(7 * number_of_sockets);

    for (const std::string& color : colors) {
        joined_colors.append("#");
        joined_colors.append(color);
    }

    return joined_colors;
}

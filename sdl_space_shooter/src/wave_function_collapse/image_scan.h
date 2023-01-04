#pragma once

#include <regex>
#include <SDL.h>
#include <vector>
#include <unordered_map>

#include "application/texture_manager.h"
#include "edge_direction_types.h"
#include "hex_color_utils.h"
#include "tile_data.h"
#include "utils/sdl_operators.h"

enum class axis_types
{
    x,
    y
};

struct texture_data
{
    SDL_Texture* texture;
    SDL_PixelFormat* format;
    int width;
    int height;
};

struct scan_iterator_data
{
    int start;
    int end;
    int opposite_axis_position;
    int step_value;
    int number_of_pixels;
    axis_types axis;
};

static scan_iterator_data get_iterator_data(edge_direction_types edge, int width, int height)
{
    scan_iterator_data scan_color_data;

    switch (edge) {
    case edge_direction_types::top:
        scan_color_data = {
            0, // start
            width, // end
            0, // opposite axis position
            1, // step direction
            width, // number of steps
            axis_types::x // axis to step
        };

        break;
    case edge_direction_types::right:
        scan_color_data = {
            0, // start
            height, // end
            width - 1, // opposite axis position
            1, // step direction
            height, // number of steps
            axis_types::y // axis to step
        };

        break;
    case edge_direction_types::bottom:
        scan_color_data = {
            width - 1, // start
            -1, // end
            height - 1, // opposite axis position
            -1, // step direction
            width, // number of steps
            axis_types::x // axis to step
        };

        break;
    case edge_direction_types::left:
    default:
        scan_color_data = {
            height - 1, // start
            -1, // end
            0, // opposite axis position
            -1, // step direction
            height, // number of steps
            axis_types::y // axis to step
        };

        break;
    }

    return scan_color_data;
}

static std::vector<SDL_Color> scan_pixels(const application::pixel_data& pixel_data, const scan_iterator_data& scan_iterator_data)
{
    std::vector<SDL_Color> colors;
    colors.reserve(scan_iterator_data.number_of_pixels);

    int x = 0;
    int y = 0;
    for (int i = scan_iterator_data.start; i != scan_iterator_data.end; i += scan_iterator_data.step_value) {
        x = scan_iterator_data.axis == axis_types::x ? i : scan_iterator_data.opposite_axis_position;
        y = scan_iterator_data.axis == axis_types::y ? i : scan_iterator_data.opposite_axis_position;

        Uint8* pixel = (Uint8*)pixel_data.pixels + y * pixel_data.pitch + x * pixel_data.format->BytesPerPixel;

        const Uint32 pixel_value = SDL_BYTEORDER == SDL_BIG_ENDIAN
            ? pixel[0] << 16 | pixel[1] << 8 | pixel[2]
            : pixel[0] | pixel[1] << 8 | pixel[2] << 16;

        Uint8 r, g, b;
        SDL_GetRGB(pixel_value, pixel_data.format, &r, &g, &b);
        colors.emplace_back(SDL_Color{ r, g, b, 1 });
    }

    return colors;
}

static SDL_Color get_dominant_color(std::vector<SDL_Color> colors_to_scan, int start, int end)
{
    int max_count = 0;
    SDL_Color most_dominant_color{};
    std::unordered_map<SDL_Color, int, utils::SDL_Color_Hash> colors_to_count;
    colors_to_count.reserve(end - start);

    // Count number of times colors is found
    for (int i = start; i < std::min(end, static_cast<int>(colors_to_scan.size())); i++) {
        colors_to_count[colors_to_scan[i]]++;
    }

    for (const auto& [color, number_of_times] : colors_to_count) {
        if (number_of_times > max_count)
        {
            most_dominant_color = color;
            max_count = number_of_times;
        }
    }

    return most_dominant_color;
}

inline tile_data scan_pixel_edges(std::string texture_path, int number_of_sockets)
{
    tile_data tile{
        texture_path,
    	number_of_sockets
    };

    const auto pixel_data = application::texture_manager::get_pixels(texture_path);
    const int& width = pixel_data.width;
    const int& height = pixel_data.height;

    std::string socket {};

    std::vector<SDL_Color> edge_colors;
    edge_colors.reserve(width * height);
    
    // For each edge of the texture (top, right, bottom. left)
	for (int edge_index = 0; edge_index < 4; edge_index++)
    {
        socket.reserve(28); // #000000 * 4

	    const scan_iterator_data iterator_data = get_iterator_data(static_cast<edge_direction_types>(edge_index), width, height);
        edge_colors = scan_pixels(
            pixel_data, 
            iterator_data
        );

        const auto number_of_pixels_in_socket_group = iterator_data.number_of_pixels / static_cast<double>(number_of_sockets);
        auto start = 0;
        auto end = static_cast<int>(ceil(number_of_pixels_in_socket_group));

        // For each socket in the image
        for (int socket_index = 0; socket_index < number_of_sockets; socket_index++)
        {
            SDL_Color color = get_dominant_color(edge_colors, start, end);
            std::string str = hex_color_utils::rgb_to_hex(color);

            socket.append(str);

            start += (socket_index % 2) == 0
                ? static_cast<int>(ceil(number_of_pixels_in_socket_group))
                : static_cast<int>(floor(number_of_pixels_in_socket_group));

            end += (socket_index % 2) == 1
                ? static_cast<int>(ceil(number_of_pixels_in_socket_group))
                : static_cast<int>(floor(number_of_pixels_in_socket_group));
        }

        tile.edges[edge_index] = socket;
        edge_colors.clear();
        socket.clear();
    }

    pixel_data.clear();

    return tile;
}

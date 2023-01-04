#pragma once
#include "image_scan.h"
#include "tile_data.h"

enum class tile_types
{
	circle,
	circuit,
	cyber_circuit,
};

static tile_data_list circle_tiles(int number_of_sockets)
{
	tile_data_list tiles;
	tiles.reserve(22);

	tile_data b = scan_pixel_edges("resources/wfc/circle/b.png", number_of_sockets);

	tile_data b_half = scan_pixel_edges("resources/wfc/circle/b_half.png", number_of_sockets);
	tile_data b_half_1 = b_half.rotate_tile(1);
	tile_data b_half_2 = b_half.rotate_tile(3);
	tile_data b_half_3 = b_half.rotate_tile(2);

	tile_data b_i = scan_pixel_edges("resources/wfc/circle/b_i.png", number_of_sockets);
	tile_data b_i_1 = b_i.rotate_tile(1);

	tile_data b_quarter = scan_pixel_edges("resources/wfc/circle/b_quarter.png", number_of_sockets);
	tile_data b_quarter_1 = b_half.rotate_tile(1);
	tile_data b_quarter_2 = b_half.rotate_tile(3);
	tile_data b_quarter_3 = b_half.rotate_tile(2);

	tile_data w = scan_pixel_edges("resources/wfc/circle/b.png", number_of_sockets);

	tile_data w_half = scan_pixel_edges("resources/wfc/circle/w_half.png", number_of_sockets);
	tile_data w_half_1 = w_half.rotate_tile(1);
	tile_data w_half_2 = w_half.rotate_tile(3);
	tile_data w_half_3 = w_half.rotate_tile(2);

	tile_data w_i = scan_pixel_edges("resources/wfc/circle/w_i.png", number_of_sockets);
	tile_data w_i_1 = w_i.rotate_tile(1);

	tile_data w_quarter = scan_pixel_edges("resources/wfc/circle/w_quarter.png", number_of_sockets);
	tile_data w_quarter_1 = w_half.rotate_tile(1);
	tile_data w_quarter_2 = w_half.rotate_tile(3);
	tile_data w_quarter_3 = w_half.rotate_tile(2);

	tiles.emplace(b);
	tiles.emplace(b_half);
	tiles.emplace(b_half_1);
	tiles.emplace(b_half_2);
	tiles.emplace(b_half_3);
	tiles.emplace(b_i);
	tiles.emplace(b_i_1);
	tiles.emplace(b_quarter);
	tiles.emplace(b_quarter_1);
	tiles.emplace(b_quarter_2);
	tiles.emplace(b_quarter_3);

	tiles.emplace(w);
	tiles.emplace(w_half);
	tiles.emplace(w_half_1);
	tiles.emplace(w_half_2);
	tiles.emplace(w_half_3);
	tiles.emplace(w_i);
	tiles.emplace(w_i_1);
	tiles.emplace(w_quarter);
	tiles.emplace(w_quarter_1);
	tiles.emplace(w_quarter_2);
	tiles.emplace(w_quarter_3);

	return tiles;
}

static tile_data_list circuit_tiles(int number_of_sockets)
{
	tile_data_list tiles;
	tiles.reserve(40);

	tile_data bridge = scan_pixel_edges("resources/wfc/circuit/bridge.png", number_of_sockets);
	tile_data bridge_1 = bridge.rotate_tile(1);

	tile_data component = scan_pixel_edges("resources/wfc/circuit/component.png", number_of_sockets);

	tile_data connection = scan_pixel_edges("resources/wfc/circuit/connection.png", number_of_sockets);
	tile_data connection_1 = connection.rotate_tile(1);
	tile_data connection_2 = connection.rotate_tile(2);
	tile_data connection_3 = connection.rotate_tile(3);

	tile_data corner = scan_pixel_edges("resources/wfc/circuit/corner.png", number_of_sockets);
	tile_data corner_1 = corner.rotate_tile(1);
	tile_data corner_2 = corner.rotate_tile(2);
	tile_data corner_3 = corner.rotate_tile(3);

	tile_data dskew = scan_pixel_edges("resources/wfc/circuit/dskew.png", number_of_sockets);
	tile_data dskew_1 = dskew.rotate_tile(1);
	tile_data dskew_2 = dskew.rotate_tile(2);
	tile_data dskew_3 = dskew.rotate_tile(3);

	tile_data skew = scan_pixel_edges("resources/wfc/circuit/skew.png", number_of_sockets);
	tile_data skew_1 = skew.rotate_tile(1);
	tile_data skew_2 = skew.rotate_tile(2);
	tile_data skew_3 = skew.rotate_tile(3);

	tile_data substrate = scan_pixel_edges("resources/wfc/circuit/substrate.png", number_of_sockets);

	tile_data t = scan_pixel_edges("resources/wfc/circuit/t.png", number_of_sockets);
	tile_data t_1 = t.rotate_tile(1);
	tile_data t_2 = t.rotate_tile(2);
	tile_data t_3 = t.rotate_tile(3);

	tile_data track = scan_pixel_edges("resources/wfc/circuit/track.png", number_of_sockets);
	tile_data track_1 = track.rotate_tile(1);

	tile_data transition = scan_pixel_edges("resources/wfc/circuit/transition.png", number_of_sockets);
	tile_data transition_1 = transition.rotate_tile(1);
	tile_data transition_2 = transition.rotate_tile(2);
	tile_data transition_3 = transition.rotate_tile(3);

	tile_data turn = scan_pixel_edges("resources/wfc/circuit/turn.png", number_of_sockets);
	tile_data turn_1 = turn.rotate_tile(1);
	tile_data turn_2 = turn.rotate_tile(2);
	tile_data turn_3 = turn.rotate_tile(3);

	tile_data viad = scan_pixel_edges("resources/wfc/circuit/viad.png", number_of_sockets);
	tile_data viad_1 = viad.rotate_tile(1);

	tile_data vias = scan_pixel_edges("resources/wfc/circuit/vias.png", number_of_sockets);
	tile_data vias_1 = vias.rotate_tile(1);

	tile_data wire = scan_pixel_edges("resources/wfc/circuit/wire.png", number_of_sockets);
	tile_data wire_1 = wire.rotate_tile(1);

	// 0-1
	tiles.emplace(bridge);
	tiles.emplace(bridge_1);

	// 2
	tiles.emplace(component);

	// 3-6
	tiles.emplace(connection);
	tiles.emplace(connection_1);
	tiles.emplace(connection_2);
	tiles.emplace(connection_3);

	// 7-10
	tiles.emplace(corner);
	tiles.emplace(corner_1);
	tiles.emplace(corner_2);
	tiles.emplace(corner_3);

	// 11-14
	tiles.emplace(dskew);
	tiles.emplace(dskew_1);
	tiles.emplace(dskew_2);
	tiles.emplace(dskew_3);

	// 15-18
	tiles.emplace(skew);
	tiles.emplace(skew_1);
	tiles.emplace(skew_2);
	tiles.emplace(skew_3);

	// 19
	tiles.emplace(substrate);

	// 20-23
	tiles.emplace(t);
	tiles.emplace(t_1);
	tiles.emplace(t_2);
	tiles.emplace(t_3);

	// 24-25
	tiles.emplace(track);
	tiles.emplace(track_1);

	// 26-29
	tiles.emplace(transition);
	tiles.emplace(transition_1);
	tiles.emplace(transition_2);
	tiles.emplace(transition_3);

	// 30-33
	tiles.emplace(turn);
	tiles.emplace(turn_1);
	tiles.emplace(turn_2);
	tiles.emplace(turn_3);

	// 34-35
	tiles.emplace(viad);
	tiles.emplace(viad_1);

	// 36-37
	tiles.emplace(vias);
	tiles.emplace(vias_1);

	// 38-39
	tiles.emplace(wire);
	tiles.emplace(wire_1);

	return tiles;
}

static tile_data_list cyber_circuit_tiles(int number_of_sockets)
{
	tile_data_list tiles;
	tiles.reserve(44);

	tile_data one = scan_pixel_edges("resources/wfc/cyber-circuit/1.png", number_of_sockets);
	tiles.emplace(one);
	tiles.emplace(one.rotate_tile(1));
	tiles.emplace(one.rotate_tile(2));
	tiles.emplace(one.rotate_tile(3));

	tile_data two = scan_pixel_edges("resources/wfc/cyber-circuit/2.png", number_of_sockets);
	tiles.emplace(two);
	tiles.emplace(two.rotate_tile(1));
	tiles.emplace(two.rotate_tile(2));
	tiles.emplace(two.rotate_tile(3));

	tile_data three = scan_pixel_edges("resources/wfc/cyber-circuit/3.png", number_of_sockets);
	tiles.emplace(three);
	tiles.emplace(three.rotate_tile(1));
	tiles.emplace(three.rotate_tile(2));
	tiles.emplace(three.rotate_tile(3));


	tile_data four = scan_pixel_edges("resources/wfc/cyber-circuit/4.png", number_of_sockets);
	tiles.emplace(four);
	tiles.emplace(four.rotate_tile(1));
	tiles.emplace(four.rotate_tile(2));
	tiles.emplace(four.rotate_tile(3));

	tile_data five = scan_pixel_edges("resources/wfc/cyber-circuit/5.png", number_of_sockets);
	tiles.emplace(five);
	tiles.emplace(five.rotate_tile(1));
	tiles.emplace(five.rotate_tile(2));
	tiles.emplace(five.rotate_tile(3));

	tile_data six = scan_pixel_edges("resources/wfc/cyber-circuit/6.png", number_of_sockets);
	tiles.emplace(six);
	tiles.emplace(six.rotate_tile(1));
	tiles.emplace(six.rotate_tile(2));
	tiles.emplace(six.rotate_tile(3));

	tile_data seven = scan_pixel_edges("resources/wfc/cyber-circuit/7.png", number_of_sockets);
	tiles.emplace(seven);
	tiles.emplace(seven.rotate_tile(1));
	tiles.emplace(seven.rotate_tile(2));
	tiles.emplace(seven.rotate_tile(3));

	tile_data eight = scan_pixel_edges("resources/wfc/cyber-circuit/8.png", number_of_sockets);
	tiles.emplace(eight);
	tiles.emplace(eight.rotate_tile(1));
	tiles.emplace(eight.rotate_tile(2));
	tiles.emplace(eight.rotate_tile(3));

	tile_data nine = scan_pixel_edges("resources/wfc/cyber-circuit/9.png", number_of_sockets);
	tiles.emplace(nine);
	tiles.emplace(nine.rotate_tile(1));
	tiles.emplace(nine.rotate_tile(2));
	tiles.emplace(nine.rotate_tile(3));

	tile_data ten = scan_pixel_edges("resources/wfc/cyber-circuit/10.png", number_of_sockets);
	tiles.emplace(ten);
	tiles.emplace(ten.rotate_tile(1));
	tiles.emplace(ten.rotate_tile(2));
	tiles.emplace(ten.rotate_tile(3));

	tile_data eleven = scan_pixel_edges("resources/wfc/cyber-circuit/11.png", number_of_sockets);
	tiles.emplace(eleven);
	tiles.emplace(eleven.rotate_tile(1));
	tiles.emplace(eleven.rotate_tile(2));
	tiles.emplace(eleven.rotate_tile(3));

	return tiles;
}

class tile_factory final
{
public:
	static tile_data_list create(tile_types type)
	{
		switch (type)
		{
		case tile_types::circle:
			return circle_tiles(get_number_of_sockets(type));

		case tile_types::circuit:
			return circuit_tiles(get_number_of_sockets(type));

		case tile_types::cyber_circuit:
		default:
			return cyber_circuit_tiles(get_number_of_sockets(type));
		}
	}
	static int get_number_of_sockets(tile_types type)
	{
		if (type == tile_types::circle)
		{
			return 1;
		} else if (type == tile_types::cyber_circuit)
		{
			return 3;
		}

		return 4;
	}
};

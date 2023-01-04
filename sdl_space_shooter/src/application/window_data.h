#pragma once

struct window_data
{
public:
	window_data(const char* name, int window_width, int window_height);

	const char* name;
	int window_width;
	int window_height;
};
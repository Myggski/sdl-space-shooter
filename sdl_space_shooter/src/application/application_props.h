#pragma once

namespace application
{
	struct application_props
	{
	public:
		application_props(const char* name, int window_width, int window_height);

		const char* name;
		int window_width;
		int window_height;
	};
}

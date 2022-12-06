#include "pch.h"
#include "application_props.h"

namespace application
{
	application_props::application_props(const char* name, int window_width, int window_height)
		: name(name), window_width(window_width), window_height(window_height) {}
}
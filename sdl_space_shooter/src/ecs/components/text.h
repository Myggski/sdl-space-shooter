#pragma once

#include "component.h"

namespace ecs
{
	namespace components
	{
		struct text : component<text, component_types::text>
		{
		public:
			text();
			text(std::string text_string, std::string font_name = "default");

		public:
			std::string text_string;
			std::string font_name; // TODO - This will probably never happen, but change this into a enum class
		};
	}
}

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

			std::string text_string;
			std::string font_name;
		};
	}
}

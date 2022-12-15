#include "pch.h"
#include "text.h"

namespace ecs
{
	namespace components
	{
		text::text()
			: text_string({}), font_name("default") { }
		text::text(std::string text_string, std::string font_name)
			: text_string(std::move(text_string)), font_name(std::move(font_name)) { }
	}
}

#pragma once

#include <cstdint>

namespace ecs
{
		using entity = uint32_t;
		using index = uint32_t;
		static constexpr auto invalid_entity_id = std::numeric_limits<index>::max();
}

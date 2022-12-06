

#pragma once

#include <cstdint>

namespace ecs
{
	using entity_id = uint32_t;
	constexpr entity_id MAX_ENTITIES = 1024;
	static constexpr auto invalid_entity_id = std::numeric_limits<entity_id>::max();
	enum class entity : uint32_t {};

}
